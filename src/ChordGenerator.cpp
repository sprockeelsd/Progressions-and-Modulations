//
// Created by Damien Sprockeels on 02/07/2024.
//

#include "../headers/ChordGenerator.hpp"


/**
 * @brief ChordGenerator
 * @param size the number of chords to be generated
 * @param tonality the tonality of the piece
 */
ChordGenerator::ChordGenerator(int size, Tonality* tonality) {
    this->size          = size;
    this->tonality      = tonality;
    chords              = IntVarArray(*this, size, FIRST_DEGREE, SEVENTH_DEGREE);
    states              = IntVarArray(*this, size, FUNDAMENTAL_STATE, THIRD_INVERSION);
    borrowedChords      = IntVarArray(*this, size-1, DIATONIC, BORROWED); /// the last chord cannot be borrowed

    /// constraints

    /// borrowed chords resolution (V/X -> X)
    for(int i = 0; i < size-1; i++){
        borrowed_chords_resolution(*this, i, chords, borrowedChords);
    }

    ///chord[i] -> chord[i+1] is possible
    for(int i = 0; i < size - 1; i++){
        element(*this, majorTonalityTransitions, expr(*this, chords[i] * 7 + chords[i+1]), 1);
    }

    /// I64-> V5/7+ (same state)
    for(int i = 0; i < size - 1; i++)
        fifth_degree_appogiatura(*this, i, chords, states);

    /// Only the V chord can be in third inversion since it is the only 4 note chord

    /// branching
    /// branch on states first because it is more restrictive
    branch(*this, borrowedChords, INT_VAR_SIZE_MIN(), INT_VAL_MIN());
    branch(*this, states, INT_VAR_SIZE_MIN(), INT_VAL_MIN());
    branch(*this, chords, INT_VAR_SIZE_MIN(), INT_VAL_MIN());
}

/**
 * @brief ChordGenerator
 * @param s a ChordGenerator object
 */
ChordGenerator::ChordGenerator(ChordGenerator &s) : Space(s){
    size = s.size;
    tonality = s.tonality;

    chords.update(*this, s.chords);
    states.update(*this, s.states);
    borrowedChords.update(*this, s.borrowedChords);
}

/**
 * @brief copy
 * @return a Space* object that is a copy of the current object
 */
Space* ChordGenerator::copy() {
    return new ChordGenerator(*this);
}

/**
 * Returns a string with each of the object's field values as integers. For debugging
 * @brief toString
 * @return a string representation of the object
 */
string ChordGenerator::toString() const{
    string txt = "";
    txt += "Chord Generator Object. \n";
    txt += "size: " + to_string(size) + "\n";
    txt += "Tonality: " + tonality->get_name() + "\n";
    txt += "Chords: " + intVarArray_to_string(chords) + "\n";
    txt += "States: " + intVarArray_to_string(states) + "\n";
    txt += "Borrowed Chords: " + intVarArray_to_string(borrowedChords) + "\n";
    return txt;
}

/**
 * Returns the object's representation in a more readable way, with degrees, state and whether the chord is borrowed
 * as strings instead of integers.
 * @brief pretty
 * @return a string representation of the object
 */
string ChordGenerator::pretty() const{
    string txt = "Chord progresion: \n";
    for(int i = 0; i < size; i++){
        txt += degreeNames.at(chords[i].val()) + " ";
    }
    txt += "\n\n More details: \n";
    for(int i = 0; i < size; i++){
        txt += degreeNames.at(chords[i].val()) + "\tin " + stateNames.at(states[i].val());
        if(i != size-1 && borrowedChords[i].val() == BORROWED)
            txt += "\t(Borrowed)";
        txt += "\n\n";
    }
    return txt;
}