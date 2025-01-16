//
// Created by Damien Sprockeels on 02/07/2024.
//

#include "../headers/ChordProgression.hpp"

/**
 * Constructor for ChordProgression objects. It initializes the object with the given parameters, links the subset arrays
 * to the main arrays in the TonalPiece class, and posts the constraints.
 * @param home the search space
 * @param start the starting position of the progression in the global piece
 * @param duration the duration of the progression
 * @param tonality the tonality of the progression
 * @param states the states of the chords
 * @param qualities the qualities of the chords
 * @param qualitiesWithoutSeventh the qualities of the chords without the seventh
 * @param rootNotes the root notes of the chords
 * @param hasSeventh whether the chord has a seventh or not
 * @param minPercentChromaticChords the minimum percentage of chromatic chords in the progression
 * @param maxPercentChromaticChords the maximum percentage of chromatic chords in the progression
 * @param minPercentSeventhChords the minimum percentage of seventh chords in the progression
 * @param maxPercentSeventhChords the maximum percentage of seventh chords in the progression
 * @return a ChordProgression object
 */
ChordProgression::
ChordProgression(Home home, int start, int duration, Tonality *tonality, IntVarArray states, IntVarArray qualities,
                 IntVarArray qualitiesWithoutSeventh, IntVarArray rootNotes, IntVarArray hasSeventh,
                 double minPercentChromaticChords, double maxPercentChromaticChords, double minPercentSeventhChords,
                 double maxPercentSeventhChords) {

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
    this->qualitiesWithoutSeventh   = IntVarArray(home, qualitiesWithoutSeventh   .slice(start, 1, duration));
    this->bassDegrees               = IntVarArray(home, duration,   FIRST_DEGREE, SEVENTH_DEGREE);
    this->rootNotes                 = IntVarArray(home, rootNotes       .slice(start, 1, duration));

    this->roots                 = IntVarArray(home, duration, FIRST_DEGREE,             SEVENTH_DEGREE);
    this->thirds                = IntVarArray(home, duration, FIRST_DEGREE,             SEVENTH_DEGREE);
    this->fifths                = IntVarArray(home, duration, FIRST_DEGREE,             SEVENTH_DEGREE);
    this->sevenths              = IntVarArray(home, duration, FIRST_DEGREE,             SEVENTH_DEGREE);

    this->isChromatic               = IntVarArray (home, duration, 0, 1);
    this->hasSeventh                = IntVarArray (home, hasSeventh.slice(start, 1, duration));

    /// constraints
    tonal_progression(home, this->duration, this->tonality, this->states, this->qualities, this->rootNotes,
                      chords, bassDegrees, isChromatic, this->hasSeventh, roots, thirds, fifths,
                      sevenths,
                      minChromaticChords, maxChromaticChords, minSeventhChords, maxSeventhChords);

    /// Optional constraints
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
    qualitiesWithoutSeventh     .update(home, s.qualitiesWithoutSeventh);
    bassDegrees                 .update(home, s.bassDegrees);
    rootNotes                   .update(home, s.rootNotes);

    roots                       .update(home, s.roots);
    thirds                      .update(home, s.thirds);
    fifths                      .update(home, s.fifths);
    sevenths                    .update(home, s.sevenths);

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
    txt += "Qualities (no seventh):\t"           + intVarArray_to_string(qualitiesWithoutSeventh) + "\n";
    txt += "Bass degrees:\t\t"                      + intVarArray_to_string(bassDegrees)    + "\n";
    txt += "Root notes:\t\t"                        + intVarArray_to_string(rootNotes)      + "\n";
    txt += "Chromatic chords:\t"                    + intVarArray_to_string(isChromatic)    + "\n";
    txt += "Seventh chords:\t\t"                    + intVarArray_to_string(hasSeventh)     + "\n";

    txt += "Roots:\t\t\t"                           + intVarArray_to_string(roots)          + "\n";
    txt += "Thirds:\t\t\t"                          + intVarArray_to_string(thirds)         + "\n";
    txt += "Fifths:\t\t\t"                          + intVarArray_to_string(fifths)         + "\n";
    txt += "Sevenths:\t\t"                        + intVarArray_to_string(sevenths)       + "\n";
    return txt;
}

/**
 * Returns the object's representation in a more readable way, with degrees and states.
 * @brief pretty
 * @return a string representation of the object
 */
string ChordProgression::pretty() const{ //todo change the representation so that it is more clear
    string txt;
    try{
        string degs, stas;
        for(int i = 0; i < duration; i++){
            degs += degreeNames     .at(chords[i].val())    + "\t";
            stas += stateNames      .at(states[i].val())    + "\t";
        }
        txt += "Chords: " + degs + "\nStates: " + stas + "\n";
    }
    catch(exception &e){
        std::cout << "Some variables are unbound in the chord progression object" << std::endl;
    }
    return txt;
}