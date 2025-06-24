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
 * formula: DegreeQualities[chords[i] * nSupportedQualities + qualities[i]] = 1
 * @param home the problem space
 * @param size the number of chords in the progression
 * @param tonality the tonality of the progression
 * @param qualities the array of chord qualities
 * @param chords the array of chord degrees
 */
void link_chords_to_qualities(const Home &home, const int size, const Tonality *tonality, IntVarArray qualities, IntVarArray chords){
    for (int i = 0; i < size; i++){
        if(tonality->get_mode() == MAJOR_MODE){
            element(home, majorDegreeQualities, expr(home, chords[i] * nSupportedQualities + qualities[i]), 1);
        }
        else if(tonality->get_mode() == MINOR_MODE){
            element(home, minorDegreeQualities, expr(home, chords[i] * nSupportedQualities + qualities[i]), 1);
        }
    }
}

/**
 * Links chord states to the degrees
 * The state of each chord is linked to its degree (I can be in fund/1st inversion, VI can be in fund,...)
 * formula: degreeStates[chords[i] * nSupportedStates + states[i]] = 1
 * @param home the problem space
 * @param size the number of chords in the progression
 * @param states the array of chord states
 * @param chords the array of chord degrees
 */
void link_chords_to_states(const Home &home, const int size, IntVarArray states, IntVarArray chords) {
    for(int i = 0; i < size; i++)
        element(home, degreeStates, expr(home, chords[i] * nSupportedStates + states[i]), 1);
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
void link_states_to_qualities(const Home &home, const int size, IntVarArray states, IntVarArray hasSeventh){
    for(int i = 0; i < size; i++)
        rel(home, expr(home, hasSeventh[i] == 0), BOT_IMP,expr(home,states[i] < THIRD_INVERSION), true);
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
void link_root_notes_to_degrees(const Home &home, const int size, Tonality *tonality, IntVarArray rootNotes, IntVarArray chords) {
    vector<int> notes;
    for (int i = FIRST_DEGREE; i <= AUGMENTED_SIXTH; i++)
        notes.push_back(tonality->get_degree_note(i));
    const IntArgs tonality_notes(notes);
    for (int i = 0; i < size; i++)
        element(home, tonality_notes, chords[i], rootNotes[i]);
}

/**
 * Links the bass degree to the corresponding chord degree and state.
 * Formula: bassBasedOnDegreeAndState[chords[i] * nSupportedStates + states[i]] = bassDegrees[i]
 * @param home the problem space
 * @param size the number of chords in the progression
 * @param states the array of chord states
 * @param bassDegrees the array of bass degrees
 * @param chords the array of chord degrees
 */
void link_bass_degrees_to_degrees_and_states(const Home &home, const int size, IntVarArray states,
    IntVarArray chords, IntVarArray bassDegrees){
    for (int i = 0; i < size; i++)
        element(home, bassBasedOnDegreeAndState, expr(home, chords[i] * nSupportedStates + states[i]), bassDegrees[i]);
}

/**
 * Links the notes constituting the chord to the chord degree
 * formula: roots[i] = bassBasedOnDegreeAndState[chords[i] * nSupportedStates + FUNDAMENTAL_STATE] etc.
 * @param home the problem space
 * @param duration the number of chords in the progression
 * @param chords the array of chord degrees
 * @param roots the array of root notes
 * @param thirds the array of third notes
 * @param fifths the array of fifth notes
 * @param sevenths the array of seventh notes
 */
void link_notes_to_degree(const Home &home, const int duration, IntVarArray chords, IntVarArray roots, IntVarArray thirds,
    IntVarArray fifths, IntVarArray sevenths) {
    for(int i = 0; i < duration; i++){
        element(home, bassBasedOnDegreeAndState, expr(home, chords[i] * nSupportedStates + FUNDAMENTAL_STATE), roots[i]);
        element(home, bassBasedOnDegreeAndState, expr(home, chords[i] * nSupportedStates + FIRST_INVERSION), thirds[i]);
        element(home, bassBasedOnDegreeAndState, expr(home, chords[i] * nSupportedStates + SECOND_INVERSION), fifths[i]);
        element(home, bassBasedOnDegreeAndState, expr(home, chords[i] * nSupportedStates + THIRD_INVERSION), sevenths[i]);
    }
}

/**
 * Link the chromatic chords array to the chords array, and constraints the number of chromatic chords to be in the range
 * [minChromaticChords, maxChromaticChords]
 * formula: isChromatic[i] == 1 <=> chords[i] >= FIVE_OF_TWO
 * formula: minChromaticChords <= sum(isChromatic) <= maxChromaticChords
 * @param home the problem space
 * @param size the number of chords in the progression
 * @param chords the array of chord degrees
 * @param qualities the array of chord qualities
 * @param isChromatic the array of chromatic chords
 * @param minChromaticChords the min number of chromatic chords we want
 * @param maxChromaticChords the max number of chromatic chords we want
 */
void chromatic_chords(const Home &home, const int size, IntVarArray chords, IntVarArray qualities, IntVarArray isChromatic,
    const int minChromaticChords, const int maxChromaticChords) {
    ///link the chromatic chords
    for (int i = 0; i < size; i++){
        rel(home, expr(home, chords[i] >= FIVE_OF_TWO), BOT_IMP, expr(home, isChromatic[i] == 1), true);
        rel(home, expr(home, chords[i] <= FIFTH_DEGREE_APPOGIATURA && chords[i] != FIFTH_DEGREE), BOT_IMP, expr(home, isChromatic[i] == 0), true);
        /// /!\ this cannot be EQV, otherwise no other chromatic chords are allowed
        rel(home, expr(home, chords[i] == FIFTH_DEGREE && qualities[i] == DIMINISHED_SEVENTH_CHORD), BOT_IMP, expr(home, isChromatic[i] == 1), true);
        rel(home, expr(home, chords[i] == FIFTH_DEGREE && qualities[i] != DIMINISHED_SEVENTH_CHORD), BOT_IMP, expr(home, isChromatic[i] == 0), true);
    }
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
void seventh_chords(const Home &home, const int size, IntVarArray qualities, IntVarArray hasSeventh, const int minSeventhChords,
    const int maxSeventhChords) {
    /// link the seventh chords
    for (int i = 0; i < size; i++)
        rel(home, expr(home, hasSeventh[i] == 1),BOT_EQV,expr(home, qualities[i] >= DOMINANT_SEVENTH_CHORD), true);
    /// count the number of seventh chords
    rel(home, sum(hasSeventh) <= maxSeventhChords);
    rel(home, sum(hasSeventh) >= minSeventhChords);
}

/**
 * Links the quality array to the quality_without_seventh array. This is useful when you need to know the general
 * quality of the chord but don't need to know if it has a seventh
 * @param home the problem space
 * @param size the number of chords in the progression
 * @param qualities the array of chord qualities
 * @param qualityWithoutSeventh the array of chord qualities without the seventh
 */
void link_qualities_to_3note_version(const Home &home, int size, IntVarArray qualities, IntVarArray qualityWithoutSeventh) {
    const IntArgs qualities_to_simple_version = {
            ///   Major        Minor        Diminished        Augmented    Dominant7
            MAJOR_CHORD, MINOR_CHORD, DIMINISHED_CHORD, AUGMENTED_CHORD, MAJOR_CHORD,
            ///  Major7       Minor7       Diminished7    Half diminished,      MinorMajor,           Augmented sixth
            MAJOR_CHORD, MINOR_CHORD, DIMINISHED_CHORD,  DIMINISHED_CHORD,     MINOR_CHORD,           AUGMENTED_CHORD
    };
    for (int i = 0; i < size; i++) {
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
void chord_transitions(const Home &home, const int size, IntVarArray chords) {
    for(int i = 0; i < size - 1; i++)
        element(home, tonalTransitions, expr(home, chords[i] * nSupportedChords + chords[i + 1]), 1);
}

/**
 * Force the last chord to be diatonic and not the seventh chord,
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
 * cases handled for now:
 *      - V65/-> I5 (formula: chords[i] = V & states[i] = 1st) => chords[i+1] = I & states[i+1] = fund)
 *      - V+4->I6 (formula: chords[i] = V & states[i] = 3rd) => chords[i+1] = I & states[i+1] = 1st)
 * @param home the problem space
 * @param size the number of chords
 * @param states the array of chord states
 * @param qualities the array of chord qualities
 * @param chords the array of chord degrees
 * @param bassDegrees the array of bass degrees
 */
void tritone_resolutions(Home home, const int size, IntVarArray states, IntVarArray qualities, IntVarArray chords,
    IntVarArray bassDegrees) {
    for(int i = 0; i < size - 1; i++){
        /// Dominant chords
        BoolVar isDominantChord(home, 0, 1);
        rel(home, isDominantChord, IRT_EQ,
            expr(home,
                expr(home, chords[i] == FIFTH_DEGREE && (qualities[i] == MAJOR_CHORD || qualities[i] == DOMINANT_SEVENTH_CHORD
                || qualities[i] == DIMINISHED_SEVENTH_CHORD)) ||
                expr(home, FIVE_OF_TWO <= chords[i] && chords[i] <= FIVE_OF_SEVEN)
            )
        );
        ///65/ -> 5
        rel(home, expr(home, isDominantChord && states[i] == FIRST_INVERSION), BOT_IMP,
            expr(home, bassDegrees[i+1] == expr(home, bassDegrees[i]+1) % 7), true);


        /// +4 -> 6
        rel(home, expr(home, isDominantChord && states[i] == THIRD_INVERSION), BOT_IMP,
            expr(home, bassDegrees[i+1] == expr(home, bassDegrees[i]-1) % 7), true);
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
    for(int i = 0; i < size; i++) {
        /// chords that don't have a seventh cannot be in third inversion
        rel(home, expr(home, qualities[i] < DOMINANT_SEVENTH_CHORD), BOT_IMP,
            expr(home, states[i] < THIRD_INVERSION), true);
        /// chords that don't have a ninth cannot be in fourth inversion
        rel(home, expr(home, qualities[i] < MINOR_NINTH_DOMINANT_CHORD), BOT_IMP,
            expr(home, states[i] < FOURTH_INVERSION), true);
    }
}

/**
 * Enforces that seventh chords are prepared correctly.
 * @param home the problem space
 * @param size the number of chords in the progression
 * @param hasSeventh the array of whether chords have a seventh
 * @param qualities the array of chord qualities
 * @param chords the array of chord degrees
 * @param roots the root notes of the chords
 * @param thirds the third notes of the chords
 * @param fifths the fifth notes of the chords
 * @param sevenths the seventh notes of the chords
 */
void seventh_chords_preparation(const Home &home, int size, IntVarArray hasSeventh, IntVarArray qualities, IntVarArray chords,
    const IntVarArray& roots, const IntVarArray& thirds, const IntVarArray& fifths, const IntVarArray& sevenths){
    for (int i = 1; i < size; i++) {
        rel(home,
            expr(home, hasSeventh[i] == 1 && qualities[i] != DOMINANT_SEVENTH_CHORD && chords[i] <= SEVENTH_DEGREE),
            BOT_IMP,
            expr(home, roots[i - 1] == sevenths[i] || thirds[i - 1] == sevenths[i] || fifths[i - 1] == sevenths[i]),
            true);
    }
}

/**
 * V/VII can only be used in minor mode
 * @param home the problem space
 * @param size the number of chords in the progression
 * @param chords the array of chord degrees
 * @param tonality the tonality of the progression
 */
void five_of_seven(const Home& home, const int size, IntVarArray chords, const Tonality* tonality) {
    if (tonality->get_mode() == MAJOR_MODE)
        for (int i = 0; i < size; i++)
            rel(home, chords[i] != FIVE_OF_SEVEN);
}

/**
 * Diminished seventh chords must be in first inversion (they are considered as a dominant ninth chord without a fundamental)
 * @param home the problem space
 * @param size the number of chords in the progression
 * @param qualities the array of chord qualities
 * @param chords the array of chord degrees
 * @param states the array of chord states
 */
void diminished_seventh_dominant_chords(const Home &home, int size, IntVarArray qualities, IntVarArray chords, IntVarArray states){
    for (int i = 0; i < size; i++)
        rel(home, expr(home, qualities[i] == DIMINISHED_SEVENTH_CHORD && chords[i] != SEVENTH_DEGREE), BOT_IMP,
            expr(home, states[i] == FIRST_INVERSION), true);
}


/***********************************************************************************************************************
 *                                            Optional Constraints (preferences)                                       *
 ***********************************************************************************************************************/

/**
 * Enforces different cadence types at a given position
 * @param home the problem space
 * @param position the position of the first chord of the cadence in the section
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
            rel(home, chords[position] == FIFTH_DEGREE && states[position] == FUNDAMENTAL_STATE);
            rel(home, chords[position + 1] == SIXTH_DEGREE && states[position + 1] == FUNDAMENTAL_STATE && hasSeventh[position + 1] == 0);
            break;
        default:                     /// Ignore unknown types
            break;
    }
}
