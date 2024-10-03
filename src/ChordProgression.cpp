//
// Created by Damien Sprockeels on 02/07/2024.
//

#include "../headers/ChordProgression.hpp"

/**
 * Chord generator constructor
 * @param home
 * @param tonality
 * @param start
 * @param duration
 * @param chordDegrees
 * @param states
 * @param qualities
 * @param rootNotes
 * @param minPercentChromaticChords
 * @param maxPercentChromaticChords
 * @param minPercentSeventhChords
 * @param maxPercentSeventhChords
 */
ChordProgression::ChordProgression(Home home, Tonality *tonality, int start, int duration, IntVarArray states,
                                   IntVarArray qualities, IntVarArray rootNotes, double minPercentChromaticChords,
                                   double maxPercentChromaticChords, double minPercentSeventhChords,
                                   double maxPercentSeventhChords) {
    this->duration                  = duration;
    this->minChromaticChords        = (int) (minPercentChromaticChords * duration);    /// converts the percentage into a number of chords
    this->maxChromaticChords        = (int) (maxPercentChromaticChords * duration);    /// converts the percentage into a number of chords
    this->minSeventhChords          = (int) (minPercentSeventhChords * duration);      /// converts the percentage into a number of chords
    this->maxSeventhChords          = (int) (maxPercentSeventhChords * duration);      /// converts the percentage into a number of chords

    this->tonality                  = tonality;

    this->chords                    = IntVarArray(home, duration, FIRST_DEGREE, AUGMENTED_SIXTH);
    this->states                    = IntVarArray(home, states          .slice(start, 1, duration));
    this->qualities                 = IntVarArray(home, qualities       .slice(start, 1, duration));
    this->bassDegrees               = IntVarArray(home, duration, FIRST_DEGREE, SEVENTH_DEGREE);
    this->rootNotes                 = IntVarArray(home, rootNotes       .slice(start, 1, duration));

    this->isChromatic               = IntVarArray (home, duration, 0, 1);
    this->hasSeventh                = IntVarArray (home, duration, 0, 1);

    /// constraints

    //todo make an options object that has a field for every parameter
    //todo link root notes with chords
    //todo add some measure of variety (number of chords used, max % of chord based on degree, ...)
    //todo add preference for state based on the chord degree (e.g. I should be often used in fund, sometimes 1st inversion, 2nd should be often in 1st inversion, ...)
    //todo check if it is more profitable to remove the seventh chords from the qualities array and to deduce them from the hasSeventh array in post-processing

    //todo add other chords (9, add6,...)?
    //todo V-> VI can only happen in fund state

    tonal_progression(home, this->duration,
                      chords, this->states, this->qualities, isChromatic, hasSeventh, bassDegrees,
                      this->rootNotes, this->tonality, minChromaticChords, maxChromaticChords, minSeventhChords,
                      maxSeventhChords);


    /*******************************************************************************************************************
     *                                            Preference constraints                                               *
     ******************************************************************************************************************/



    /// branching
    Rnd r(1U);
    branch(home, chords, INT_VAR_SIZE_MIN(), INT_VAL_RND(r));
}

/**
 * Copy constructor
 * @param home
 * @param s
 */
ChordProgression::ChordProgression(Home home, ChordProgression &s){
    duration                    = s.duration;
    minChromaticChords          = s.minChromaticChords;
    maxChromaticChords          = s.maxChromaticChords;
    minSeventhChords            = s.minSeventhChords;
    maxSeventhChords            = s.maxSeventhChords;

    tonality                    = s.tonality;
    chords                      .update(home, s.chords);
    states                      .update(home, s.states);
    qualities                   .update(home, s.qualities);
    bassDegrees                 .update(home, s.bassDegrees);
    rootNotes                   .update(home, s.rootNotes);

    isChromatic                 .update(home, s.isChromatic);
    hasSeventh                  .update(home, s.hasSeventh);
}

/**
 * Returns a string with each of the object's field values as integers. For debugging
 * @brief toString
 * @return a string representation of the object
 */
string ChordProgression::toString() const{ //todo change the representation so that the different tonalities are visible
    string txt;
    txt += "------------------ Chord Progression Object. Parameters: ------------------\n\n";
    txt += "size: " + to_string(duration) + "\n";
    txt += "Tonality: " + tonality->get_name() + "\n";
    txt += "Chromatic chords number between " + to_string(minChromaticChords) + " and " + to_string(maxChromaticChords) + "\n";
    txt += "Seventh chords number between " + to_string(minSeventhChords) + " and " + to_string(maxSeventhChords) + "\n";

    txt += "\n--------------------------Solution--------------------------\n";
    txt += "Details: \n";
    txt += "Number of chromatic chords between " + to_string(minChromaticChords) + " and " + to_string(maxChromaticChords) + "\n";
    txt += "Number of seventh chords between " + to_string(minSeventhChords) + " and " + to_string(maxSeventhChords) + "\n";
    txt += "Chords:\t\t\t" + intVarArray_to_string(chords) + "\n";
    txt += "States:\t\t\t" + intVarArray_to_string(states) + "\n";
    txt += "Qualities:\t\t" + intVarArray_to_string(qualities) + "\n";
    txt += "Bass notes:\t\t" + intVarArray_to_string(bassDegrees) + "\n";
    txt += "Root notes:\t\t" + intVarArray_to_string(rootNotes) + "\n";
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
string ChordProgression::pretty() const{ //todo change the representation so that the different tonalities are visible
    string txt = "Chord progression: \n";
//    for(int j = 0; j < tonalities.size(); j++){
//        txt += tonalities[j]->get_name() + " ";
//        for(int i = tonalitiesStarts[j]; i < tonalitiesStarts[j] + tonalitiesDurations[j]; i++)
//            txt += degreeNames.at(chords[i].val()) + " ";
//        txt += "\n";
//    }
//
//    txt += "\n\nMore details: \n";
//    for(int j = 0; j < tonalities.size(); j++){
//        txt += tonalities[j]->get_name() + "\n";
//        for(int i = tonalitiesStarts[j]; i < tonalitiesStarts[j] + tonalitiesDurations[j]; i++){
//            txt += degreeNames.at(chords[i].val()) + "\tin " + stateNames.at(states[i].val()) + "\t (" +
//                    chordQualityNames.at(qualities[i].val()) + ")\n\n";
//        }
//    }
    return txt + "\n\n";
}