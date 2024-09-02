//
// Created by Damien Sprockeels on 03/07/2024.
//

#include "../headers/Constraints.hpp"

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