//
// Created by Damien Sprockeels on 03/07/2024.
//

#include "../headers/Constraints.hpp"

/**
 * Enforces that chord progression are valid
 * 1. chord[i] -> chord[i+1] is possible (matrix)
 * formula: tonalTransitions[chords[i] * nSupportedChords + chords[i + 1]] = 1
 * @param home the problem space
 * @param size the number of chords
 * @param chords the array of chord degrees
 */
void chord_transitions(const Home& home, int size, IntVarArray chords){
    for(int i = 0; i < size - 1; i++)
        element(home, tonalTransitions, expr(home, chords[i] * nSupportedChords + chords[i + 1]), 1);
}

/**
 * Links chord qualities to the degrees
 * 2. The quality of each chord is linked to its degree (V is major/7, I is major,...)
 * formula: majorDegreeQualities[chords[i] * nSupportedQualities + qualities[i]] = 1
 * @param home the problem space
 * @param chords the array of chord degrees
 * @param qualities the array of chord qualities
 */
void link_chords_to_qualities(const Home& home, IntVarArray chords, IntVarArray qualities) {
    for (int i = 0; i < qualities.size(); i++)
        element(home, majorDegreeQualities, expr(home, chords[i] * nSupportedQualities + qualities[i]), 1);
}

/**
 * Links chord states to the degrees
 * 3. The state of each chord is linked to its degree (I can be in fund/1st inversion, VI can be in fund,...)
 * formula: majorDegreeStates[chords[i] * nSupportedStates + states[i]] = 1
 * @param home the problem space
 * @param chords the array of chord degrees
 * @param states the array of chord states
 */
void link_chords_to_states(const Home& home, IntVarArray chords, IntVarArray states) {
    for(int i = 0; i < states.size(); i++)
        element(home, majorDegreeStates, expr(home, chords[i] * nSupportedStates + states[i]), 1);
}

/**
 * 4. The state of each chord is linked to its quality (7th chords can be in 3rd inversion, etc)
 * formula: qualitiesToStates[qualities[i] * nSupportedStates + states[i]] = 1
 * @param home the problem space
 * @param qualities the array of chord qualities
 * @param states the array of chord states
 */
void link_states_to_qualities(const Home& home, IntVarArray qualities, IntVarArray states) {
    for(int i = 0; i < states.size(); i++)
        element(home, qualitiesToStates, expr(home, qualities[i] * nSupportedStates + states[i]), 1);
}


/**
 * Posts the constraint that the fifth degree appogiatura (V64-I in second inversion) must resolve to the fifth degree
 * chord in fundamental state (V5-V7+)
 * @param home the problem space
 * @param curPos the current position in the chord progression
 * @param chords the variables for the chords of the progression
 * @param states the variables for the state of the chords
 */
void fifth_degree_appogiatura(const Home& home, int curPos, IntVarArray chords, IntVarArray states){
    ///V64-> V5/7+
    rel(home,
        expr(home, expr(home, chords[curPos] == FIRST_DEGREE) && expr(home, states[curPos] == SECOND_INVERSION)),
        BOT_IMP,
        expr(home, expr(home, chords[curPos+1] == FIFTH_DEGREE) && expr(home, states[curPos+1] == FUNDAMENTAL_STATE)),
        true);
}