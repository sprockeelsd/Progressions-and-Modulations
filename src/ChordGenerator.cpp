//
// Created by Damien Sprockeels on 02/07/2024.
//

#include "../headers/ChordGenerator.hpp"


/**
 * Chord generator constructor
 * @param s the number of chords to be generated
 * @param tonalities the tonality of the piece
 * @param maxPercentChromaticChords lower bound on the percentage of chromatic chords in the progression
 * @param maxPercentChromaticChords upper bound on the percentage of chromatic chords in the progression
 * @param minPercentSeventhChords lower bound on the percentage of seventh chords in the progression
 * @param maxPercentSeventhChords upper bound on the percentage of seventh chords in the progression
 */
ChordGenerator::ChordGenerator(int s, vector<Tonality *> tonalities, vector<int> tonalitiesStarts,
                               vector<int> tonalitiesDurations, vector<int> modulationTypes,
                               vector<int> modulationStarts, double minPercentChromaticChords,
                               double maxPercentChromaticChords, double minPercentSeventhChords,
                               double maxPercentSeventhChords) {
    this->size                  = s;
    this->tonalities            = tonalities; ///todo check the moving thing later
    this->tonalitiesStarts      = tonalitiesStarts;
    this->tonalitiesDurations   = tonalitiesDurations;
    this->modulationTypes       = modulationTypes;
    this->modulationStarts      = modulationStarts;

    this->minChromaticChords    = (int) (minPercentChromaticChords * size);    /// converts the percentage into a number of chords
    this->maxChromaticChords    = (int) (maxPercentChromaticChords * size);    /// converts the percentage into a number of chords
    this->minSeventhChords      = (int) (minPercentSeventhChords * size);      /// converts the percentage into a number of chords
    this->maxSeventhChords      = (int) (maxPercentSeventhChords * size);      /// converts the percentage into a number of chords

    this->chords                = IntVarArray(*this, size, FIRST_DEGREE, AUGMENTED_SIXTH);
    this->states                = IntVarArray(*this, size, FUNDAMENTAL_STATE, THIRD_INVERSION);
    this->qualities             = IntVarArray(*this, size, MAJOR_CHORD, MINOR_MAJOR_SEVENTH_CHORD);
    this->bassNotes             = IntVarArray(*this, size, FIRST_DEGREE, SEVENTH_DEGREE);

    this->isChromatic           = IntVarArray(*this, size, 0, 1);
    this->hasSeventh            = IntVarArray(*this, size, 0, 1);

    /// constraints

    //todo add some measure of variety (number of chords used, max % of chord based on degree, ...)
    //todo add preference for state based on the chord degree (e.g. I should be often used in fund, sometimes 1st inversion, 2nd should be often in 1st inversion, ...)
    //todo check if it is more profitable to remove the seventh chords from the qualities array and to deduce them from the hasSeventh array in post-processing

    //todo add other chords (9, add6,...)?
    //todo V-> VI can only happen in fund state

    for(int i = 0; i < tonalities.size(); i++)
        tonal_progression(*this, tonalitiesDurations[i], tonalitiesStarts[i],
                          chords, states, qualities, isChromatic, hasSeventh, bassNotes, minChromaticChords,
                          maxChromaticChords, minSeventhChords, maxSeventhChords);

    for(int i = 0; i < modulationStarts.size(); i++){
        switch (modulationTypes[i]){
            case PERFECT_CADENCE_MODULATION:
                cadence(*this, modulationStarts[i], PERFECT_CADENCE, chords, states, hasSeventh);
                break;
            case PIVOT_CHORD_MODULATION:

                break;
            default:
                break;
        }
    }


    /*******************************************************************************************************************
     *                                            Preference constraints                                               *
     ******************************************************************************************************************/



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

    tonalities              = s.tonalities;
    tonalitiesStarts       = s.tonalitiesStarts;
    tonalitiesDurations    = s.tonalitiesDurations;
    modulationTypes         = s.modulationTypes;
    modulationStarts        = s.modulationStarts;

    chords                  .update(*this, s.chords);
    states                  .update(*this, s.states);
    qualities               .update(*this, s.qualities);
    bassNotes               .update(*this, s.bassNotes);

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
string ChordGenerator::toString() const{ //todo change the representation so that the different tonalities are visible
    string txt;
    txt += "------------------ Chord Generator Object. Parameters: ------------------\n\n";
    txt += "size: " + to_string(size) + "\n";
    txt += "Tonalities: ";
    for(auto t: tonalities)           txt += t->get_name() + " ";
    txt += "\nTonality starts: ";
    for(auto s: tonalitiesStarts)           txt += to_string(s) + " ";
    txt += "\nTonality durations: ";
    for(auto d: tonalitiesDurations)        txt += to_string(d) + " ";
    txt += "\nModulation types: ";
    for(auto m: modulationTypes)            txt += to_string(m) + " ";
    txt += "\nModulation starts: ";
    for(auto m: modulationStarts)           txt += to_string(m) + " ";
    txt += "\n--------------------------Solution--------------------------\n";
    txt += "Details: \n";
    txt += "Number of chromatic chords between " + to_string(minChromaticChords) + " and " + to_string(maxChromaticChords) + "\n";
    txt += "Number of seventh chords between " + to_string(minSeventhChords) + " and " + to_string(maxSeventhChords) + "\n";
    txt += "Chords:\t\t\t" + intVarArray_to_string(chords) + "\n";
    txt += "States:\t\t\t" + intVarArray_to_string(states) + "\n";
    txt += "Qualities:\t\t" + intVarArray_to_string(qualities) + "\n";
    txt += "Bass note:\t\t" + intVarArray_to_string(bassNotes) + "\n";
    txt += "Chromatic chords:\t" + intVarArray_to_string(isChromatic) + "\n";
    txt += "Seventh chords:\t\t" + intVarArray_to_string(hasSeventh) + "\n";
    return txt;
}

/**
 * Returns the object's representation in a more readable way, with degrees, state and whether the chord is borrowed
 * as strings instead of integers.
 * @brief pretty
 * @return a string representation of the object
 */
string ChordGenerator::pretty() const{ //todo change the representation so that the different tonalities are visible
    string txt = "Chord progression: \n";
    for(int j = 0; j < tonalities.size(); j++){
        txt += tonalities[j]->get_name() + " ";
        for(int i = tonalitiesStarts[j]; i < tonalitiesStarts[j] + tonalitiesDurations[j]; i++)
            txt += degreeNames.at(chords[i].val()) + " ";
        txt += "\n";
    }

    txt += "\n\nMore details: \n";
    for(int j = 0; j < tonalities.size(); j++){
        txt += tonalities[j]->get_name() + "\n";
        for(int i = tonalitiesStarts[j]; i < tonalitiesStarts[j] + tonalitiesDurations[j]; i++){
            txt += degreeNames.at(chords[i].val()) + "\tin " + stateNames.at(states[i].val()) + "\t (" +
                    chordQualityNames.at(qualities[i].val()) + ")\n\n";
        }
    }
    return txt + "\n\n";
}