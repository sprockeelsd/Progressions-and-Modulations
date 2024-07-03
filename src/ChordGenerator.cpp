//
// Created by Damien Sprockeels on 02/07/2024.
//

#include "../headers/ChordGenerator.hpp"

ChordGenerator::ChordGenerator(int size, Tonality* tonality) {
    this->size          = size;
    this->tonality      = tonality;
    chords              = IntVarArray(*this, size, FIRST_DEGREE, SEVENTH_DEGREE);
    states              = IntVarArray(*this, size, FUNDAMENTAL_STATE, THIRD_INVERSION);
    qualities           = IntVarArray(*this, size, MAJOR_CHORD, DOMINANT_SEVENTH_CHORD);

    /// constraints

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
    branch(*this, qualities, INT_VAR_SIZE_MIN(), INT_VAL_MIN());
    branch(*this, states, INT_VAR_SIZE_MIN(), INT_VAL_MIN());
    branch(*this, chords, INT_VAR_SIZE_MIN(), INT_VAL_MIN());
}

ChordGenerator::ChordGenerator(ChordGenerator &s) : Space(s){
    size = s.size;
    tonality = s.tonality;

    chords.update(*this, s.chords);
    states.update(*this, s.states);
    qualities.update(*this, s.qualities);
}

Space* ChordGenerator::copy() {
    return new ChordGenerator(*this);
}

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

string ChordGenerator::pretty() const{
    string txt = "Chord progresion: \n";
    for(int i = 0; i < size; i++){
        txt += degreeNames.at(chords[i].val()) + " ";
    }
    txt += "\n\n More details: \n";
    for(int i = 0; i < size; i++){
        txt += degreeNames.at(chords[i].val()) + "\tin " + stateNames.at(states[i].val());
        if(qualities[i].val() != tonality->get_chord_quality(chords[i].val())){
            txt += "\t(" + chordQualityNames.at(qualities[i].val()) + ")";
        }
        txt += "\n\n";
    }
    return txt;
}