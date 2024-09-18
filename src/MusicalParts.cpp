//
// Created by Damien Sprockeels on 18/09/2024.
//

#include "../headers/MusicalParts.hpp"

using namespace Gecode;

void tonal_progression(const Home &home, int size, int startPosition, IntVarArray &chords, IntVarArray &states,
                       IntVarArray &qualities, IntVarArray &isChromatic, IntVarArray &hasSeventh,
                       int minChromaticChords, int maxChromaticChords, int minSeventhChords, int maxSeventhChords) {
    ///1. chord[i] -> chord[i+1] is possible (matrix)
    chord_transitions(home, size, startPosition, chords);

    ///2. The quality of each chord is linked to the degree it is (V is major/7, I is major,...)
    link_chords_to_qualities(home, size, startPosition, chords, qualities);

    ///3. The state of each chord is linked to the degree it is (I can be in fund/1st inversion, VI can be in fund,...)
    link_chords_to_states(home, size, startPosition, chords, states);

    ///4. The state of each chord is linked to its quality (7th chords can be in 3rd inversion, etc)
    link_states_to_qualities(home, size, startPosition, states, hasSeventh);

    ///5. Link the chromatic chords and count them so that there are exactly nChromaticChords
    chromatic_chords(home, size, startPosition, chords, isChromatic, minChromaticChords, maxChromaticChords);

    ///6. Link the seventh chords and count them so that there are exactly nSeventhChords
    seventh_chords(home, size, startPosition, hasSeventh, qualities, minSeventhChords, maxSeventhChords);

    ///7. The chord progression cannot end on something other than a diatonic chord (also not seventh degree) todo change that with the modulations?
    last_chord_cst(home, size, chords);

    ///8. I64-> V5/7+ (same state)
    fifth_degree_appogiatura(home, size, startPosition, chords, states, qualities);

    ///9. bII should be in first inversion todo maybe make this a preference?
    flat_II_cst(home, size, startPosition, chords, states);

    ///10. If two successive chords are the same degree, they cannot have the same state or the same quality
    ///11. The same degree cannot happen more than twice successively
    successive_chords_with_same_degree(home, size, startPosition, chords, states, qualities);

    ///12. Tritone resolutions should be allowed with the states todo test this for all possible cases (V65, V+4, V/X 65/, ...)
    tritone_resolutions(home, size, startPosition, chords, states);

    ///13. Fifth degree chord cannot be in second inversion if it is not dominant seventh
    fifth_degree(home, size, startPosition, chords, states, qualities);
}
