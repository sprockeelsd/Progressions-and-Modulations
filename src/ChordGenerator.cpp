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
    this->size                  = size;
    this->tonality              = tonality;
    this->chords                = IntVarArray(*this, size, FIRST_DEGREE, SEVENTH_DEGREE);
    this->states                = IntVarArray(*this, size, FUNDAMENTAL_STATE, THIRD_INVERSION);
    this->qualities             = IntVarArray(*this, size, MAJOR_CHORD, MINOR_MAJOR_SEVENTH_CHORD);

    vector<int> t_qualities;
    for(int i = FIRST_DEGREE; i <= SEVENTH_DEGREE; i++)
        t_qualities.push_back(tonality->get_chord_quality(i));
    IntArgs t_q(t_qualities);

    /// constraints
    //todo link qualities to chord degrees somehow
    //todo link states to qualities (3note chord cannot have 3rd inversion etc)
    //todo link states to degrees (/!\ to tritone resolution that can affect state in some cases (V+4->I6 for example))
    //todo force tension chords to resolve
    //todo allow anything that can go to V to go to Ida
    //todo allow anything that can go to X to go to V/X unless X is VII
    //todo force V/X to resolve to X
    //todo force I64 to go to V5/7+
    //todo force bII to be in first inversion
    //todo add control over borrowed chords (V/...) and chromatic chords (bII,6te aug,...) with cost and preferences



    ///1. chord[i] -> chord[i+1] is possible (matrix)
    //todo check that it is working correctly, especially for V/x chords
    for(int i = 0; i < size - 1; i++){
        element(*this, majorTonalityTransitions, expr(*this, chords[i] * 7 + chords[i+1]), 1);
    }

    ///2. borrowed chords are major or dominant 7th

    /// I64-> V5/7+ (same state) ///todo redo this with the new matrix
    ///for(int i = 0; i < size - 1; i++)
        ///fifth_degree_appogiatura(*this, i, chords, states);

    /// 3 notes chords can be in fund, 1st or 2nd inversion and 4 note chords can be in fund, 1st, 2nd or 3rd inversion


    /// branching
    /// branch on states first because it is more restrictive
    branch(*this, qualities, INT_VAR_SIZE_MIN(), INT_VAL_MIN());
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
    qualities.update(*this, s.qualities);
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
    txt += "Qualities: " + intVarArray_to_string(qualities) + "\n";
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
        txt += "\n\n";
    }
    return txt;
}