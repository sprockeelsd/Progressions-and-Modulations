//
// Created by Damien Sprockeels on 18/09/2024.
//

#include "../headers/MusicalParts.hpp"

using namespace Gecode;

/**
 * Posts all the constraints that a tonal chord progression must respect in a given tonality.
 * @param home the problem space
 * @param size the number of chords in the progression
 * @param tonality the tonality of the progression
 * @param states the states of the chords
 * @param qualities the qualities of the chords
 * @param rootNotes the root notes of the chords
 * @param chords the degrees of the chords
 * @param bassDegrees the degree of the note at the bass
 * @param isChromatic whether each chord is chromatic or not
 * @param hasSeventh whether each chord has a seventh or not
 * @param roots the root notes of the chords
 * @param thirds the third notes of the chords
 * @param fifths the fifth notes of the chords
 * @param sevenths the seventh notes of the chords
 * @param minChromaticChords the minimum number of chromatic chords in the progression
 * @param maxChromaticChords the maximum number of chromatic chords in the progression
 * @param minSeventhChords the minimum number of seventh chords in the progression
 * @param maxSeventhChords the maximum number of seventh chords in the progression
 */
void tonal_progression(const Home& home, const int size, Tonality *tonality, const IntVarArray &states, const IntVarArray &qualities,
                       const IntVarArray &rootNotes, const IntVarArray &chords, const IntVarArray &bassDegrees, const IntVarArray &isChromatic,
                       const IntVarArray &hasSeventh, const IntVarArray& roots, const IntVarArray& thirds, const IntVarArray& fifths,
                       const IntVarArray& sevenths, const int minChromaticChords, const int maxChromaticChords, const int minSeventhChords,
                       const int maxSeventhChords) {
    ///1. chord[i] -> chord[i+1] is possible
    chord_transitions(home, size, chords);


    ///2. Link notes to degrees
    link_notes_to_degree(home, size, chords, roots, thirds, fifths, sevenths);

    ///3. The quality of each chord is linked to the degree it is (V is major/7, I is major,...)
    link_chords_to_qualities(home, size, tonality, qualities, chords);

    ///4. The state of each chord is linked to its degree (I can be in fund/1st inversion, VI can be in fund,...)
    link_chords_to_states(home, size, states, chords);

    ///5. The state of each chord is linked to its quality (7th chords can be in 3rd inversion, etc)
    link_states_to_qualities(home, size, states, hasSeventh);

    ///6. Link root notes to degrees in this tonality
    link_root_notes_to_degrees(home, size, tonality, rootNotes, chords);

    ///7. link root note to chord + degree;
    link_bass_degrees_to_degrees_and_states(home, size, states, chords, bassDegrees);

    ///8. Link the chromatic chords and count them so that they are in the appropriate range
    chromatic_chords(home, size, chords, qualities, isChromatic, minChromaticChords, maxChromaticChords);

    ///9. Link the seventh chords and count them so that they are in the appropriate range
    seventh_chords(home, size, qualities, hasSeventh, minSeventhChords, maxSeventhChords);

    ///10. Vda-> V5/7+ (fundamental state)
    fifth_degree_appogiatura(home, size, states, qualities, chords);

    ///11. bII should be in first inversion todo maybe make this a preference?
    flat_II_cst(home, size, states, chords);

    ///12. If two successive chords are the same degree, they cannot have the same state or the same quality
    ///13. The same degree cannot happen more than twice successively
    successive_chords_with_same_degree(home, size, states, qualities, chords);

    ///14. Tritone resolutions should be allowed with the states
    tritone_resolutions(home, size, states, qualities, chords, bassDegrees);

    ///15. Chords cannot be in third inversion if they don't have a seventh
    chord_states_and_qualities(home, size, states, qualities);

    ///16. 7èmes d'espèces doivent être préparées
    seventh_chords_preparation(home, size, hasSeventh, qualities, chords, roots, thirds, fifths, sevenths);

    ///17. V/VII can only be used in minor mode
    five_of_seven(home, size, chords, tonality);

    ///18. Diminished seventh chords
    diminished_seventh_dominant_chords(home, size, qualities, chords, states);

}

// todo preferences

//todo see if this is still relevant in this case. I think it should be in TonalPiece and not in ChordProgression, or at least only in the final tonal progression
//    ///7. The chord progression cannot end on something other than a diatonic chord (also not seventh degree) todo change that with the modulations?
//    last_chord_cst(home, size, chords);

///18. todo add that in minor mode, the only accepted secondary dominants are V/IV and V/V. V/II is never allowed, other secondary dominants can only be used to modulate to the relative major