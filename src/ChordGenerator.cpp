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
    this->chords                = IntVarArray(*this, size, FIRST_DEGREE, AUGMENTED_SIXTH);
    this->states                = IntVarArray(*this, size, FUNDAMENTAL_STATE, THIRD_INVERSION);
    this->qualities             = IntVarArray(*this, size, MAJOR_CHORD, MINOR_MAJOR_SEVENTH_CHORD);

    vector<int> t_qualities;
    for(int i = FIRST_DEGREE; i <= SEVENTH_DEGREE; i++)
        t_qualities.push_back(tonality->get_chord_quality(i));
    IntArgs t_q(t_qualities);

    /// constraints
    //todo link states to qualities (3note chord cannot have 3rd inversion etc)
    //todo link states to degrees (/!\ to tritone resolution that can affect state in some cases (V+4->I6 for example))
    //todo force I64 to go to V5/7+
    //todo force bII to be in first inversion
    //todo add control over borrowed chords (V/...) and chromatic chords (bII,6te aug,...) with cost and preferences
    //todo borrowed chords are dominant seventh or major
    //todo V degree chord can only be in 2nd or 3rd inversion if they are dominant 7th
    //todo maybe make an array saying if the chord has a seventh or not

    ///1. chord[i] -> chord[i+1] is possible (matrix)
    ///formula: tonalTransitions[chords[i] * nSupportedChords + chords[i + 1]] = 1
    for(int i = 0; i < size - 1; i++)
        element(*this, tonalTransitions, expr(*this, chords[i] * nSupportedChords + chords[i + 1]), 1);


    ///2. The quality of each chord is linked to the degree it is (V is major/7, I is major,...)
    ///formula: majorDegreeQualities[chords[i] * nSupportedQualities + qualities[i]] = 1
    for(int i = 0; i < qualities.size(); i++)
        element(*this,  majorDegreeQualities, expr(*this,chords[i] * nSupportedQualities + qualities[i]), 1);


    ///3. The state of each chord is linked to the degree it is (I can be in fund/1st inversion, VI can be in fund,...)
    ///formula: majorDegreeStates[chords[i] * nSupportedStates + states[i]] = 1
    for(int i = 0; i < states.size(); i++)
        element(*this, majorDegreeStates, expr(*this, chords[i] * nSupportedStates + states[i]), 1);


    /// I64-> V5/7+ (same state) ///todo redo this with the new matrix
    ///for(int i = 0; i < size - 1; i++)
        ///fifth_degree_appogiatura(*this, i, chords, states);

    /// 3 notes chords can be in fund, 1st or 2nd inversion and 4 note chords can be in fund, 1st, 2nd or 3rd inversion


    /// branching
    branch(*this, chords, INT_VAR_SIZE_MIN(), INT_VAL_MIN());
    branch(*this, qualities, INT_VAR_SIZE_MIN(), INT_VAL_MIN());
    branch(*this, states, INT_VAR_SIZE_MIN(), INT_VAL_MIN());
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
    string txt = "Chord progression: \n";
    for(int i = 0; i < size; i++){
        txt += degreeNames.at(chords[i].val()) + " ";
    }
    txt += "\n\n More details: \n";
    for(int i = 0; i < size; i++){
        txt += degreeNames.at(chords[i].val()) + "\tin " + stateNames.at(states[i].val()) + "\t (" +
                chordQualityNames.at(qualities[i].val()) + ")";
        txt += "\n\n";
    }
    return txt;
}