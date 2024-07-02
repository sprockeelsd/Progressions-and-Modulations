//
// Created by Damien Sprockeels on 02/07/2024.
//

#include "../headers/ChordGenerator.hpp"

ChordGenerator::ChordGenerator(int size) {
    this->size = size;
    chords = IntVarArray(*this, size, FIRST_DEGREE, SEVENTH_DEGREE);

    branch(*this, chords, INT_VAR_SIZE_MIN(), INT_VAL_MIN());
}

ChordGenerator::ChordGenerator(ChordGenerator &s) : Space(s){
    size = s.size;
    chords.update(*this, s.chords);
}

Space* ChordGenerator::copy() {
    return new ChordGenerator(*this);
}
