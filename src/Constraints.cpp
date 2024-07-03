//
// Created by Damien Sprockeels on 03/07/2024.
//

#include "../headers/Constraints.hpp"

void fifth_degree_appogiatura(Home home, int curPos, IntVarArray chords, IntVarArray states){
    ///V64-> V5/7+
    rel(home,
        expr(home, expr(home, chords[curPos] == FIRST_DEGREE) && expr(home, states[curPos] == SECOND_INVERSION)),
        BOT_IMP,
        expr(home, expr(home, chords[curPos+1] == FIFTH_DEGREE) && expr(home, states[curPos+1] == FUNDAMENTAL_STATE)),
        true);
}