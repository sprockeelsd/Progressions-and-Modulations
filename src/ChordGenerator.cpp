//
// Created by Damien Sprockeels on 02/07/2024.
//

#include "../headers/ChordGenerator.hpp"


/**
 * Chord generator constructor
 * @param s the number of chords to be generated
 * @param tonality the tonality of the piece
 * @param maxPercentChromaticChords lower bound on the percentage of chromatic chords in the progression
 * @param maxPercentChromaticChords upper bound on the percentage of chromatic chords in the progression
 * @param minPercentSeventhChords lower bound on the percentage of seventh chords in the progression
 * @param maxPercentSeventhChords upper bound on the percentage of seventh chords in the progression
 */
ChordGenerator::ChordGenerator(int s, Tonality *tonality, double minPercentChromaticChords,
                               double maxPercentChromaticChords, double minPercentSeventhChords,
                               double maxPercentSeventhChords) {
    this->size                  = s;
    this->tonality              = tonality;
    this->minChromaticChords    = (int) (minPercentChromaticChords * size);    /// converts the percentage into a number of chords
    this->maxChromaticChords    = (int) (maxPercentChromaticChords * size);    /// converts the percentage into a number of chords
    this->minSeventhChords      = (int) (minPercentSeventhChords * size);      /// converts the percentage into a number of chords
    this->maxSeventhChords      = (int) (maxPercentSeventhChords * size);      /// converts the percentage into a number of chords

    this->chords                = IntVarArray(*this, size, FIRST_DEGREE, AUGMENTED_SIXTH);
    this->states                = IntVarArray(*this, size, FUNDAMENTAL_STATE, THIRD_INVERSION);
    this->qualities             = IntVarArray(*this, size, MAJOR_CHORD, MINOR_MAJOR_SEVENTH_CHORD);

    this->isChromatic           = IntVarArray(*this, size, 0, 1);
    this->hasSeventh            = IntVarArray(*this, size, 0, 1);

    /// constraints

    //todo add some measure of variety (number of chords used, max % of chord based on degree, ...)
    //todo add preference for state based on the chord degree (e.g. I should be often used in fund, sometimes 1st inversion, 2nd should be often in 1st inversion, ...)
    //todo check if it is more profitable to remove the seventh chords from the qualities array and to deduce them from the hasSeventh array in post-processing
    //todo test the chromatic and seventh chords constraints

    //todo add other chords (9, add6,...)?
    //todo V-> VI can only happen in fund state

    ///1. chord[i] -> chord[i+1] is possible (matrix)
    chord_transitions(*this, size, chords);

    ///2. The quality of each chord is linked to the degree it is (V is major/7, I is major,...)
    link_chords_to_qualities(*this, chords, qualities);

    ///3. The state of each chord is linked to the degree it is (I can be in fund/1st inversion, VI can be in fund,...)
    link_chords_to_states(*this, chords, states);

    ///4. The state of each chord is linked to its quality (7th chords can be in 3rd inversion, etc)
    link_states_to_qualities(*this, states, hasSeventh);

    ///5. Link the chromatic chords and count them so that there are exactly nChromaticChords
    chromatic_chords(*this, size, chords, isChromatic, minChromaticChords, maxChromaticChords);

    ///6. Link the seventh chords and count them so that there are exactly nSeventhChords
    seventh_chords(*this, size, hasSeventh, qualities, minSeventhChords, maxSeventhChords);

    ///7. The chord progression cannot end on something other than a diatonic chord (also not seventh degree)
    last_chord_cst(*this, size, chords);

    ///8. I64-> V5/7+ (same state)
    fifth_degree_appogiatura(*this, size, chords, states, qualities);

    ///9. bII should be in first inversion todo maybe make this a preference?
    flat_II_cst(*this, size, chords, states);

    ///10. If two successive chords are the same degree, they cannot have the same state or the same quality
    ///11. The same degree cannot happen more than twice successively
    successive_chords_with_same_degree(*this, size, chords, states, qualities);

    ///12. Tritone resolutions should be allowed with the states todo test this for all possible cases (V65, V+4, V/X 65/, ...)
    //tritone_resolutions(*this, size, chords, states);

    ///13. Fifth degree chord cannot be in second inversion if it is not dominant seventh
    fifth_degree(*this, size, chords, states, qualities);


    /*******************************************************************************************************************
     *                                            Preference constraints                                               *
     ******************************************************************************************************************/

    /// cadences
//    cadence(*this, size / 2, HALF_CADENCE, chords, states, hasSeventh);
//    cadence(*this, size - 2, PERFECT_CADENCE, chords, states, hasSeventh);

    /// branching
    Rnd r(1U);
    branch(*this, chords, INT_VAR_SIZE_MIN(), INT_VAL_RND(r));
    branch(*this, qualities, INT_VAR_SIZE_MIN(), INT_VAL_MIN());
    branch(*this, states, INT_VAR_SIZE_MIN(), INT_VAL_MIN());
}

/**
 * @brief ChordGenerator
 * @param s a ChordGenerator object
 */
ChordGenerator::ChordGenerator(ChordGenerator &s) : Space(s){
    size                    = s.size;
    minChromaticChords      = s.minChromaticChords;
    maxChromaticChords      = s.maxChromaticChords;
    minSeventhChords        = s.minSeventhChords;
    maxSeventhChords        = s.maxSeventhChords;
    tonality                = s.tonality;

    chords                  .update(*this, s.chords);
    states                  .update(*this, s.states);
    qualities               .update(*this, s.qualities);

    isChromatic             .update(*this, s.isChromatic);
    hasSeventh              .update(*this, s.hasSeventh);
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
    string txt;
    txt += "------------------ Chord Generator Object. Parameters: ------------------\n\n";
    txt += "size: " + to_string(size) + "\n";
    txt += "Tonality: " + tonality->get_name() + "\n";
    txt += "Number of chromatic chords between " + to_string(minChromaticChords) + " and " + to_string(maxChromaticChords) + "\n";
    txt += "Number of seventh chords between " + to_string(minSeventhChords) + " and " + to_string(maxSeventhChords) + "\n";
    txt += "Chords: " + intVarArray_to_string(chords) + "\n";
    txt += "States: " + intVarArray_to_string(states) + "\n";
    txt += "Qualities: " + intVarArray_to_string(qualities) + "\n";
    txt += "Chromatic chords: " + intVarArray_to_string(isChromatic) + "\n";
    txt += "Seventh chords: " + intVarArray_to_string(hasSeventh) + "\n";
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
//        if(hasSeventh[i].val() == 1)
//            txt += " with a seventh";
        txt += "\n\n";
    }
    return txt;
}