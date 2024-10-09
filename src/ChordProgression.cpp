//
// Created by Damien Sprockeels on 02/07/2024.
//

#include "../headers/ChordProgression.hpp"

/**
 * Constructor for ChordProgression objects. It initializes the object with the given parameters, and posts the
 * constraints. It also posts the branching on the chords array. Branching for the other arrays is handled in the
 * TonalPiece class
 * @param home the search space
 * @param tonality the tonality of the progression
 * @param start the starting position of the progression in the global piece
 * @param duration the duration of the progression
 * @param states the states of the chords
 * @param qualities the qualities of the chords
 * @param rootNotes the root notes of the chords
 * @param minPercentChromaticChords the minimum percentage of chromatic chords in the progression
 * @param maxPercentChromaticChords the maximum percentage of chromatic chords in the progression
 * @param minPercentSeventhChords the minimum percentage of seventh chords in the progression
 * @param maxPercentSeventhChords the maximum percentage of seventh chords in the progression
 * @return a ChordProgression object
 */
 ChordProgression::
ChordProgression(Home home, int start, int duration, Tonality *tonality, IntVarArray states, IntVarArray qualities,
IntVarArray rootNotes, double minPercentChromaticChords, double maxPercentChromaticChords,
double minPercentSeventhChords, double maxPercentSeventhChords) {

    this->start                     = start;
    this->duration                  = duration;
    this->minChromaticChords        = (int) (minPercentChromaticChords  * duration);    /// converts % into a number
    this->maxChromaticChords        = (int) (maxPercentChromaticChords  * duration);
    this->minSeventhChords          = (int) (minPercentSeventhChords    * duration);
    this->maxSeventhChords          = (int) (maxPercentSeventhChords    * duration);

    this->tonality                  = tonality;

    this->chords                    = IntVarArray(home, duration,   FIRST_DEGREE, AUGMENTED_SIXTH);
    this->states                    = IntVarArray(home, states          .slice(start, 1, duration));
    this->qualities                 = IntVarArray(home, qualities       .slice(start, 1, duration));
    this->bassDegrees               = IntVarArray(home, duration,   FIRST_DEGREE, SEVENTH_DEGREE);
    this->rootNotes                 = IntVarArray(home, rootNotes       .slice(start, 1, duration));

    this->isChromatic               = IntVarArray (home, duration, 0, 1);
    this->hasSeventh                = IntVarArray (home, duration, 0, 1);

    //todo make an options object that has a field for every parameter
    //todo link with Diatony
    //todo add some measure of variety (number of chords used, max % of chord based on degree, ...)
    //todo add preference for state based on the chord degree (e.g. I should be often used in fund, sometimes 1st inversion, 2nd should be often in 1st inversion, ...)
    //todo check if it is more profitable to remove the seventh chords from the qualities array and to deduce them from the hasSeventh array in post-processing
    //todo add other chords (9, add6,...)?
    //todo V-> VI can only happen in fund state

    /// constraints
    tonal_progression(home, this->duration, this->tonality, this->states, this->qualities, this->rootNotes,
                      chords,bassDegrees, isChromatic,hasSeventh,
                      minChromaticChords, maxChromaticChords, minSeventhChords, maxSeventhChords);

    /// Optional constraints
    /// cadences
//    rel(home, chords[0], IRT_EQ, SECOND_DEGREE);
//    cadence(home, duration / 2, HALF_CADENCE, states, chords, hasSeventh);
    //cadence(home, startPosition + size - 2, PERFECT_CADENCE, chords, states, hasSeventh);

    /// branching
    Rnd r(1U);
    //branch(home, chords, INT_VAR_SIZE_MIN(), INT_VAL_RND(r));
}

/**
 * Copy constructor
 * @param home the search space
 * @param s a ChordProgression object
 */
ChordProgression::ChordProgression(Home home, ChordProgression &s){
    start                       = s.start;
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
string ChordProgression::toString() const{
    string txt;
    txt += "-------------------------------- Chord Progression Object. -------------------------------------------\n\n";
    txt += "Parameters: \n";
    txt += "size: "                                 + to_string(duration)                   + "\n";
    txt += "Tonality: "                             + tonality->get_name()                      + "\n";
    txt += "Chromatic chords number between "       + to_string(minChromaticChords)         +
            " and "                                 + to_string(maxChromaticChords)         + "\n";
    txt += "Seventh chords number between "         + to_string(minSeventhChords)           +
            " and "                                 + to_string(maxSeventhChords)           + "\n";

    txt += "\n--------------------------Solution--------------------------\n";
    txt += "Details: \n";
    txt += "Number of chromatic chords between "    + to_string(minChromaticChords)         +
            " and "                                 + to_string(maxChromaticChords)         + "\n";
    txt += "Number of seventh chords between "      + to_string(minSeventhChords)           +
            " and "                                 + to_string(maxSeventhChords)           + "\n";
    txt += "Chords:\t\t\t"                          + intVarArray_to_string(chords)         + "\n";
    txt += "States:\t\t\t"                          + intVarArray_to_string(states)         + "\n";
    txt += "Qualities:\t\t"                         + intVarArray_to_string(qualities)      + "\n";
    txt += "Bass degrees:\t\t"                        + intVarArray_to_string(bassDegrees)    + "\n";
    txt += "Root notes:\t\t"                        + intVarArray_to_string(rootNotes)      + "\n";
    txt += "Chromatic chords:\t"                    + intVarArray_to_string(isChromatic)    + "\n";
    txt += "Seventh chords:\t\t"                    + intVarArray_to_string(hasSeventh)     + "\n";
    return txt;
}

/**
 * Returns the object's representation in a more readable way, with degrees and states.
 * @brief pretty
 * @return a string representation of the object
 */
string ChordProgression::pretty() const{ //todo change the representation so that the different tonalities are visible
    string txt;
    txt += "-------------------------------- Chord Progression Object. -------------------------------------------\n\n";
    try{
        string degs, stas;
        for(int i = 0; i < duration; i++){
            degs += degreeNames     .at(chords[i].val())    + "\t";
            stas += stateNames      .at(states[i].val())    + "\t";
        }
        txt += "Chords: " + degs + "\nStates: " + stas + "\n";
    }
    catch(exception &e){
        std::cout << "Some variables are unbound" << std::endl;
    }
    return txt + "\n\n";
}