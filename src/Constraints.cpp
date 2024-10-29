//
// Created by Damien Sprockeels on 03/07/2024.
//

#include "../headers/Constraints.hpp"

/***********************************************************************************************************************
 *                                        Linker functions                                                             *
 ***********************************************************************************************************************/

/**
 * Links the chord qualities to the degrees
 * The quality of each chord is linked to its degree (V is major/7, I is major,...)
 * formula: majorDegreeQualities[chords[i] * nSupportedQualities + qualities[i]] = 1
 * todo also make it work for minor tonalities
 * @param home the problem space
 * @param size the number of chords in the progression
 * @param qualities the array of chord qualities
 * @param chords the array of chord degrees
 */
void link_chords_to_qualities(const Home &home, int size, IntVarArray qualities, IntVarArray chords) {
    for (int i = 0; i < size; i++)
        element(home, majorDegreeQualities, expr(home, chords[i] * nSupportedQualities + qualities[i]), 1);
}

/**
 * Links chord states to the degrees
 * The state of each chord is linked to its degree (I can be in fund/1st inversion, VI can be in fund,...)
 * formula: majorDegreeStates[chords[i] * nSupportedStates + states[i]] = 1
 * todo also make it work for minor tonalities
 * @param home the problem space
 * @param size the number of chords in the progression
 * @param states the array of chord states
 * @param chords the array of chord degrees
 */
void link_chords_to_states(const Home &home, int size, IntVarArray states, IntVarArray chords) {
    for(int i = 0; i < size; i++)
        element(home, majorDegreeStates, expr(home, chords[i] * nSupportedStates + states[i]), 1);
}

/**
 * Links chord states to qualities
 * The state of each chord is linked to its quality (3 note chords cannot be in 3rd inversion)
 * formula: hasSeventh[i] == 0 => states[i] < THIRD_INVERSION
 * @param home the problem space
 * @param size the number of chords in the progression
 * @param states the array of chord states
 * @param hasSeventh the array of whether chords have a seventh
 */
void link_states_to_qualities(const Home &home, int size, IntVarArray states, IntVarArray hasSeventh) {
    for(int i = 0; i < size; i++)
        rel(home, expr(home, hasSeventh[i] == 0), BOT_IMP,expr(home,states[i] < THIRD_INVERSION), true);
//        element(home, qualitiesToStates, expr(home, qualities[i] * nSupportedStates + states[i]), 1);
}

/**
 * Links the root note to the corresponding degree. It makes the connection between tonalities by referring directly to notes
 * formula: rootNotes[i] = tonality->get_degree_note(chords[i])
 * @param home the problem space
 * @param size the number of chords in the progression
 * @param tonality the tonality of the progression
 * @param rootNotes the slice of the rootNote array corresponding to this tonality
 * @param chords the array of chord degrees
 */
void link_root_notes_to_degrees(const Home &home, int size, Tonality *tonality, IntVarArray rootNotes, IntVarArray chords) {
    vector<int> notes;
    for (int i = FIRST_DEGREE; i <= AUGMENTED_SIXTH; i++)
        notes.push_back(tonality->get_degree_note(i));
    IntArgs tonality_notes(notes);
    for (int i = 0; i < size; i++)
        element(home, tonality_notes, chords[i], rootNotes[i]);
}

/**
 * Links the bass degree to the corresponding chord degree and state.
 * formula: bassBasedOnDegreeAndState[chords[i] * nSupportedStates + states[i]] = bassDegrees[i]
 * @param home the problem space
 * @param size the number of chords in the progression
 * @param states the array of chord states
 * @param bassDegrees the array of bass degrees
 * @param chords the array of chord degrees
 */
void link_bass_degrees_to_degrees_and_states(const Home &home, int size, IntVarArray states, IntVarArray chords,
                                             IntVarArray bassDegrees) {
    for (int i = 0; i < size; i++)
        element(home, bassBasedOnDegreeAndState, expr(home, chords[i] * nSupportedStates + states[i]), bassDegrees[i]);
}

/**
 * Link the chromatic chords array to the chords array, and constraints the number of chromatic chords to be in the range
 * [minChromaticChords, maxChromaticChords]
 * formula: isChromatic[i] == 1 <=> chords[i] >= FIVE_OF_TWO
 * formula: minChromaticChords <= sum(isChromatic) <= maxChromaticChords
 * @param home the problem space
 * @param size the number of chords in the progression
 * @param chords the array of chord degrees
 * @param isChromatic the array of chromatic chords
 * @param minChromaticChords the min number of chromatic chords we want
 * @param maxChromaticChords the max number of chromatic chords we want
 */
void chromatic_chords(const Home &home, int size, IntVarArray chords, IntVarArray isChromatic, int minChromaticChords,
                      int maxChromaticChords) {
    ///link the chromatic chords
    for (int i = 0; i < size; i++)
        rel(home, expr(home, isChromatic[i] == 1),BOT_EQV,expr(home, chords[i] >= FIVE_OF_TWO), true);
    ///count the number of chromatic chords
    rel(home, sum(isChromatic) <= maxChromaticChords);
    rel(home, sum(isChromatic) >= minChromaticChords);
}

/**
 * Link the seventh chords and constraints the number of seventh chords to be in the range [minSeventhChords, maxSeventhChords]
 * formula: hasSeventh[i] == 1 <=> qualities[i] >= DOMINANT_SEVENTH_CHORD
 * formula: minSeventhChords <= sum(hasSeventh) <= maxSeventhChords
 * @param home the problem space
 * @param size the number of chords in the progression
 * @param qualities the array of chord qualities
 * @param hasSeventh the array of seventh chords
 * @param minSeventhChords the min number of seventh chords we want
 * @param maxSeventhChords the max number of seventh chords we want
 */
void seventh_chords(const Home &home, int size, IntVarArray qualities, IntVarArray hasSeventh, int minSeventhChords,
                    int maxSeventhChords) {
    /// link the seventh chords
    for (int i = 0; i < size; i++)
        rel(home, expr(home, hasSeventh[i] == 1),BOT_EQV,expr(home, qualities[i] >= DOMINANT_SEVENTH_CHORD), true);
    /// count the number of seventh chords
    rel(home, sum(hasSeventh) <= maxSeventhChords);
    rel(home, sum(hasSeventh) >= minSeventhChords);
}

/**
 * Links the qualities array to the quality_without_seventh array. This is useful when you need to know the general
 * quality of the chord but don't need to know if it has a seventh
 * @param home the problem space
 * @param size the number of chords in the progression
 * @param qualities the array of chord qualities
 * @param qualityWithoutSeventh the array of chord qualities without the seventh
 */
void link_qualities_to_3note_version(const Home &home, int size, IntVarArray qualities, IntVarArray qualityWithoutSeventh) {
    for (int i = 0; i < size; i++) {
        IntArgs qualities_to_simple_version = {
                ///Major            Minor       Diminished        Augmented    Dominant7
                MAJOR_CHORD, MINOR_CHORD, DIMINISHED_CHORD, AUGMENTED_CHORD, MAJOR_CHORD,
                ///  Major7       Minor7       Diminished7        MinorMajor
                MAJOR_CHORD, MINOR_CHORD, DIMINISHED_CHORD, MINOR_CHORD,
        };
        element(home, qualities_to_simple_version, qualities[i], qualityWithoutSeventh[i]);
    }
}

/***********************************************************************************************************************
 *                                            General Constraints                                                      *
 ***********************************************************************************************************************/

/**
 * Enforces that the chord progression is valid. That is, each transition between successive chords is possible
 * chord[i] -> chord[i+1] is possible (matrix)
 * formula: tonalTransitions[chords[i] * nSupportedChords + chords[i + 1]] = 1
 * @param home the problem space
 * @param size the number of chords in this tonality
 * @param chords the array of chord degrees
 */
void chord_transitions(const Home &home, int size, IntVarArray chords) {
    for(int i = 0; i < size - 1; i++)
        element(home, tonalTransitions, expr(home, chords[i] * nSupportedChords + chords[i + 1]), 1);
}

/**
 * Force the last chord to be diatonic and not the seventh chord
 * The chord progression cannot end on something other than a diatonic chord (also not seventh degree)
 * formula: chords[size - 1] < SEVENTH_DEGREE
 * @param home the problem space
 * @param size the number of chords
 * @param chords the array of chord degrees
 */
void last_chord_cst(const Home& home, int size, const IntVarArray& chords){
    rel(home, chords[size - 1] < SEVENTH_DEGREE);
}

/**
 * Enforces that the fifth degree appogiatura (Vda) is followed by the fifth degree as a major or dominant seventh chord
 * in fundamental state (V5/7+)
 * formula: chords[i] == I => states[i+1] == fund && qualities[i+1] == M/7
 * @param home the problem space
 * @param size the number of chords
 * @param states the array of chord states
 * @param qualities the array of chord qualities
 * @param chords the array of chord degrees
 */
void fifth_degree_appogiatura(const Home &home, int size, IntVarArray states, IntVarArray qualities, IntVarArray chords) {
    for(int i = 0; i< size - 1; i++)
        rel(home, expr(home,chords[i] == FIFTH_DEGREE_APPOGIATURA),
            BOT_IMP,
            expr(home, states[i+1] == FUNDAMENTAL_STATE &&
            (qualities[i+1] == MAJOR_CHORD || qualities[i+1] == DOMINANT_SEVENTH_CHORD)), true);
}

/**
 * bII should be in first inversion todo maybe make this a preference?
 * @param home the problem space
 * @param size the number of chords
 * @param states the array of chord states
 * @param chords the array of chord degrees
 */
void flat_II_cst(const Home &home, int size, IntVarArray states, IntVarArray chords) {
    for(int i = 0; i < size; i++)
        rel(home, expr(home, chords[i] == FLAT_TWO), BOT_IMP,expr(home,states[i] == FIRST_INVERSION),
            true);
}

/**
 * If two successive chords are the same degree, they cannot have the same state or the same quality
 * formula: chords[i] = chords[i+1] => states[i] != states[i+1] || qualities[i] != qualities[i+1]
 * The same degree cannot happen more than twice successively
 * formula: chords[i] == chords[i+1] => chords[i+2] != chords[i]
 * @param home the problem space
 * @param size the number of chords
 * @param states the array of chord states
 * @param qualities the array of chord qualities
 * @param chords the array of chord degrees
 */
void successive_chords_with_same_degree(const Home &home, int size, IntVarArray states, IntVarArray qualities,
                                        IntVarArray chords) {
    ///If two successive chords are the same degree, they cannot have the same state or the same quality
    for (int i = 0; i < size - 1; i++)
        rel(home, expr(home, chords[i] == chords[i + 1]), BOT_IMP,
            expr(home, states[i] != states[i + 1] || qualities[i] != qualities[i + 1]), true);
    ///The same degree cannot happen more than twice successively
    for(int i = 0; i < size-2; i++)
        rel(home, expr(home, chords[i] == chords[i+1]), BOT_IMP,
            expr(home, chords[i+2] != chords[i]), true);
}

/**
 * Makes sure the state of the chords allows for tritone resolutions in the cases where it is necessary
 * cases handeld for now:
 * Dominant chords in fundamental state, first or second inversion must go to a chord in fundamental state to allow for tritone resolution
 * Dominant chords in third inversion must go to a chord in first inversion to allow for tritone resolution
 * todo maybe allow them to go to V/x as well
 * todo test this a lot, for all kinds of variations (also with borrowed chords)
 * todo express it with quality (dominant seventh chord) and state => resolution
 * @param home the problem space
 * @param size the number of chords
 * @param states the array of chord states
 * @param chords the array of chord degrees
 */
void tritone_resolutions(Home home, int size, IntVarArray states, IntVarArray qualities, IntVarArray chords) {
    for(int i = 0; i < size - 1; i++){
        /// Dominant chords
        BoolVar isDominantChord(home, 0, 1);
        rel(home, isDominantChord, IRT_EQ,
            expr(home,
                expr(home, chords[i] == FIFTH_DEGREE && qualities[i] == MAJOR_CHORD || qualities[i] == DOMINANT_SEVENTH_CHORD) ||
                expr(home, FIVE_OF_TWO <= chords[i] && chords[i] <= FIVE_OF_SIX)
            )
        );
        ///7+-65/-+6 -> 5
        rel(home, expr(home, isDominantChord && states[i] < THIRD_INVERSION), BOT_IMP,
            expr(home, states[i+1] == FUNDAMENTAL_STATE), true);


        /// +4 -> 6
        rel(home, expr(home, isDominantChord && states[i] == THIRD_INVERSION), BOT_IMP,
            expr(home, states[i+1] == FIRST_INVERSION), true);
    }
}

/**
 * Chords that don't have a seventh cannot be in third inversion
 * formula: chords[i] == V && qualities[i] < DOMINANT_SEVENTH_CHORD => states[i] != SECOND_INVERSION todo update
 * @param home the problem space
 * @param size the number of chords
 * @param states the array of chord states
 * @param qualities the array of chord qualities
 */
void chord_states_and_qualities(const Home &home, int size, IntVarArray states, IntVarArray qualities) {
    for(int i = 0; i < size; i++)
        rel(home, expr(home, qualities[i] < DOMINANT_SEVENTH_CHORD), BOT_IMP,
            expr(home, states[i] < THIRD_INVERSION), true);
}


/***********************************************************************************************************************
 *                                            Optional Constraints (preferences)                                       *
 ***********************************************************************************************************************/

/**
 * Enforces different cadence types at a given position
 * @param home the problem space
 * @param position the position of the first chord of the cadence
 * @param type the type of cadence
 * @param states the array of chord states
 * @param chords the array of chord degrees
 * @param hasSeventh the array of seventh chords
 */
void cadence(const Home &home, int position, int type, IntVarArray states, IntVarArray chords, IntVarArray hasSeventh) {
    switch (type){
        case PERFECT_CADENCE:        /// V7+/9-I5. The dominant chord can have a 7th but it is not mandatory, the I chord cannot
            rel(home, chords[position] == FIFTH_DEGREE && states[position] == FUNDAMENTAL_STATE);
            rel(home, chords[position + 1] == FIRST_DEGREE && states[position + 1] == FUNDAMENTAL_STATE && hasSeventh[position + 1] == 0);
            break;
        case PLAGAL_CADENCE:         /// IV-I without a seventh on both chords
            rel(home, chords[position] == FOURTH_DEGREE && states[position] == FUNDAMENTAL_STATE && hasSeventh[position] == 0);
            rel(home, chords[position + 1] == FIRST_DEGREE && states[position + 1] == FUNDAMENTAL_STATE && hasSeventh[position + 1] == 0);
            break;
        case HALF_CADENCE:           /// V5/7+. The dominant chord can have a 7th but it is not mandatory
            rel(home, chords[position] == FIFTH_DEGREE && states[position] == FUNDAMENTAL_STATE);
            break;
        case DECEPTIVE_CADENCE:      /// V-VI. The dominant chord can have a 7th but it is not mandatory, the VI chord cannot
            rel(home, (chords[position] == FIFTH_DEGREE || chords[position] == SEVEN_DIMINISHED) && states[position] == FUNDAMENTAL_STATE);
            rel(home, chords[position + 1] == SIXTH_DEGREE && states[position + 1] == FUNDAMENTAL_STATE && hasSeventh[position + 1] == 0);
            break;
        default:                     /// Ignore unknown types
            break;
    }
}
