//
// Created by Damien Sprockeels on 02/07/2024.
//

#include "../headers/ChordGenerator.hpp"


/**
 * @brief ChordGenerator
 * @param s the number of chords to be generated
 * @param tonality the tonality of the piece
 */
ChordGenerator::ChordGenerator(int s, Tonality *tonality, double percentChromaticChords) {
    this->size                  = s;
    this->tonality              = tonality;
    this->nChromaticChords      = (int) (percentChromaticChords * size); /// converts the percentage into a number of chords

    this->chords                = IntVarArray(*this, size, FIRST_DEGREE, AUGMENTED_SIXTH);
    this->states                = IntVarArray(*this, size, FUNDAMENTAL_STATE, THIRD_INVERSION);
    this->qualities             = IntVarArray(*this, size, MAJOR_CHORD, MINOR_MAJOR_SEVENTH_CHORD);

    this->isChromatic           = IntVarArray(*this, size, 0, 1);

    ///todo check if this is still used
    vector<int> t_qualities;
    for(int i = FIRST_DEGREE; i <= SEVENTH_DEGREE; i++)
        t_qualities.push_back(tonality->get_chord_quality(i));
    IntArgs t_q(t_qualities);

    /// constraints
    //todo see if using a matrix interface is cleaner or not (it is not better for element constraints so probably not)
    //todo link states to degrees (/!\ to tritone resolution that can affect state in some cases (V+4->I6 for example))
    //todo force I64 to go to V5/7+
    //todo force bII to be in first inversion
    //todo add control over borrowed chords (V/...) and chromatic chords (bII,6te aug,...) with cost and preferences
    //todo borrowed chords are dominant seventh or major
    //todo V degree chord can only be in 2nd or 3rd inversion if they are dominant 7th
    //todo maybe make an array saying if the chord has a seventh or not
    //todo the chord progression cannot end on Ida nor on a V/x (basically, not on an "ornament" chord)
    //todo two successive chords cannot have the same state if they have the same degree

    ///1. chord[i] -> chord[i+1] is possible (matrix)
    ///formula: tonalTransitions[chords[i] * nSupportedChords + chords[i + 1]] = 1
    chord_transitions(*this, size, chords);


    ///2. The quality of each chord is linked to the degree it is (V is major/7, I is major,...)
    ///formula: majorDegreeQualities[chords[i] * nSupportedQualities + qualities[i]] = 1
    link_chords_to_qualities(*this, chords, qualities);


    ///3. The state of each chord is linked to the degree it is (I can be in fund/1st inversion, VI can be in fund,...)
    ///formula: majorDegreeStates[chords[i] * nSupportedStates + states[i]] = 1
    link_chords_to_states(*this, chords, states);

    ///4. The state of each chord is linked to its quality (7th chords can be in 3rd inversion, etc)
    ///formula: qualitiesToStates[qualities[i] * nSupportedStates + states[i]] = 1
    link_states_to_qualities(*this, qualities, states);

    ///5. Link the chromatic chords and count them so that there are exactly nChromaticChords
    chromatic_chords(*this, size, chords, isChromatic, nChromaticChords);

    ///6. The chord progression cannot end on something other than a diatonic chord (also not seventh degree)
    last_chord_cst(*this, size, chords);


    ///6. I64-> V5/7+ (same state)
    fifth_degree_appogiatura(*this, size, chords, states, qualities);

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
    nChromaticChords = s.nChromaticChords;
    tonality = s.tonality;

    chords.update(*this, s.chords);
    states.update(*this, s.states);
    qualities.update(*this, s.qualities);

    isChromatic.update(*this, s.isChromatic);
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
    txt += "Number of chromatic chords: " + to_string(nChromaticChords) + "\n";
    txt += "Chords: " + intVarArray_to_string(chords) + "\n";
    txt += "States: " + intVarArray_to_string(states) + "\n";
    txt += "Qualities: " + intVarArray_to_string(qualities) + "\n";
    txt += "Chromatic chords: " + intVarArray_to_string(isChromatic) + "\n";
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