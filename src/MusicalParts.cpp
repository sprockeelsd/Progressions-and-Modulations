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
 * @param minChromaticChords the minimum number of chromatic chords in the progression
 * @param maxChromaticChords the maximum number of chromatic chords in the progression
 * @param minSeventhChords the minimum number of seventh chords in the progression
 * @param maxSeventhChords the maximum number of seventh chords in the progression
 */
void tonal_progression(Home home, int size, Tonality *tonality, IntVarArray &states, IntVarArray &qualities,
                       IntVarArray &rootNotes, IntVarArray &chords, IntVarArray &bassDegrees, IntVarArray &isChromatic,
                       IntVarArray &hasSeventh, const IntVarArray& roots, const IntVarArray& thirds, const IntVarArray& fifths,
                       const IntVarArray& sevenths, int minChromaticChords, int maxChromaticChords, int minSeventhChords,
                       int maxSeventhChords) {
    ///1. chord[i] -> chord[i+1] is possible (matrix)
    chord_transitions(home, size, chords);

    /// Link notes to degrees
    link_notes_to_degree(home, size, chords, roots, thirds, fifths, sevenths);

    ///2. The quality of each chord is linked to the degree it is (V is major/7, I is major,...)
    link_chords_to_qualities(home, size, tonality, qualities, chords);

    ///3. The state of each chord is linked to its degree (I can be in fund/1st inversion, VI can be in fund,...)
    link_chords_to_states(home, size, states, chords);

    ///4. The state of each chord is linked to its quality (7th chords can be in 3rd inversion, etc)
    link_states_to_qualities(home, size, states, hasSeventh);

    ///5. Link root notes to degrees in this tonality
    link_root_notes_to_degrees(home, size, tonality, rootNotes, chords);

    ///6. link root note to chord + degree;
    link_bass_degrees_to_degrees_and_states(home, size, states, chords, bassDegrees);

    ///7. Link the chromatic chords and count them so that they are in the appropriate range
    chromatic_chords(home, size, chords, isChromatic, minChromaticChords, maxChromaticChords);

    ///8. Link the seventh chords and count them so that they are in the appropriate range
    seventh_chords(home, size, qualities, hasSeventh, minSeventhChords, maxSeventhChords);

    //todo see if this is still relevant in this case. I think it should be in TonalPiece and not in ChordProgression, or at least only in the final tonal progression
//    ///7. The chord progression cannot end on something other than a diatonic chord (also not seventh degree) todo change that with the modulations?
//    last_chord_cst(home, size, chords);
//
    ///9. V64-> V5/7+ (same state)
    fifth_degree_appogiatura(home, size, states, qualities, chords);

    ///10. bII should be in first inversion todo maybe make this a preference?
    flat_II_cst(home, size, states, chords);

    ///11. If two successive chords are the same degree, they cannot have the same state or the same quality
    ///12. The same degree cannot happen more than twice successively
    successive_chords_with_same_degree(home, size, states, qualities, chords);

    ///13. Tritone resolutions should be allowed with the states
    tritone_resolutions(home, size, states, qualities, chords, bassDegrees);

    ///14. Chords cannot be in third inversion if they don't have a seventh
    chord_states_and_qualities(home, size, states, qualities);

    ///15. 7èmes d'espèces doivent être préparées
    seventh_chords_preparation(home, size, hasSeventh, qualities, chords, roots, thirds, fifths, sevenths);

    ///16. V/VII can only be used in minor mode
    if(tonality->get_mode() == MAJOR_MODE)
        for(int i = 0; i < size; i++)
            rel(home, chords[i] != FIVE_OF_SEVEN);

    ///17. Diminished seventh chords
    for(int i = 0; i < size; i++){
        rel(home, expr(home, qualities[i] == DIMINISHED_SEVENTH_CHORD && chords[i] != SEVENTH_DEGREE), BOT_IMP,
            expr(home, states[i] == FIRST_INVERSION), true);
        /// + has Seventh = 1 + check that
        //todo problème quand c'est un accord chromatique qui est septième diminuée
    }
}