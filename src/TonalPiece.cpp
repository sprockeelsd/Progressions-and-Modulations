//
// Created by Damien Sprockeels on 30/09/2024.
//

#include <utility>

#include "../headers/TonalPiece.hpp"

/**
 * Constructor for TonalPiece objects. It initializes the object with the given parameters, as well as the other
 * objects that post constraints. It also posts the branching for the global arrays. That branching is posted after
 * the ChordProgression objects have posted theirs.
 * @param size the total size of the piece in terms of number of chords
 * @param tonalities a vector of Tonality objects for each tonality of the piece
 * @param tonalitiesStarts a vector of integers representing the starting position of each tonality. They can overlap (modulations)
 * @param tonalitiesDurations a vector of integers representing the duration of each tonality
 * @param modulationTypes a vector of integers representing the type of modulation between the tonalities
 * @param modulationStarts a vector of integers representing the starting position of each modulation
 * @return a TonalPiece object
 */
TonalPiece::
TonalPiece(int size, const vector<Tonality *> &tonalities, vector<int> tonalitiesStarts,
           vector<int> tonalitiesDurations, vector<int> modulationTypes, vector<int> modulationStarts,
           vector<int> modulationEnds) :
           size(size), tonalities(tonalities), tonalitiesStarts(tonalitiesStarts),
           tonalitiesDurations(tonalitiesDurations), modulationTypes(modulationTypes), modulationStarts(modulationStarts),
           modulationEnds(modulationEnds){

    this->states                = IntVarArray(*this, size, FUNDAMENTAL_STATE,   THIRD_INVERSION);
    this->qualities             = IntVarArray(*this, size, MAJOR_CHORD,         MINOR_MAJOR_SEVENTH_CHORD);
    this->rootNotes             = IntVarArray(*this, size, C,                   B);

    /// Create the ChordProgression objects for each tonality, and post the constraints
    progressions.reserve(tonalities.size());
    for (int i = 0; i < tonalities.size(); i++){
        progressions.push_back(
                new ChordProgression(*this,
                                     this->tonalitiesStarts[i],
                                     this->tonalitiesDurations[i],
                                     this->tonalities[i],
                                     states, qualities, rootNotes,
                                     0, 1,
                                     0, 1)
                );
    }

    modulations.reserve(modulationTypes.size());
    for(int i = 0; i < modulationTypes.size(); i++){
        modulations.push_back(
                new Modulation(*this, modulationTypes[i], modulationStarts[i], modulationEnds[i],
                               progressions[i], progressions[i+1])
                );
    }

    /// Create the Modulation objects for each modulation, and post the constraints

    /** The branching on chord degrees is performed first, through the ChordProgression objects.
     * Then it is performed on the global arrays if it is necessary. That means that the branching on degrees is done
     * in order of appearance of the tonalities. Maybe this needs to change? But then it has to be done differently
     * because the chord degrees are not available in this main class. */

    Rnd r(1U);
    for(auto p : progressions)
        branch(*this, p->getChords(), INT_VAR_SIZE_MIN(), INT_VAL_RND(r));
    branch(*this, states,       INT_VAR_SIZE_MIN(), INT_VAL_MIN());
    branch(*this, qualities,    INT_VAR_SIZE_MIN(), INT_VAL_MIN());
    branch(*this, rootNotes,    INT_VAR_SIZE_MIN(), INT_VAL_MIN());
    //todo move sevenths here because it is not tonality dependent
}

/**
 * @brief Copy constructor
 * @param s a ChordProgression object pointer
 * Returns a TonalPiece object that is equivalent to s
 */
TonalPiece::TonalPiece(TonalPiece &s) : Space(s){
    size                        = s.size;
    tonalities                  = s.tonalities;
    tonalitiesStarts            = s.tonalitiesStarts;
    tonalitiesDurations         = s.tonalitiesDurations;
    modulationTypes             = s.modulationTypes;
    modulationStarts            = s.modulationStarts;
    modulationEnds              = s.modulationEnds;
    states                      .update(*this, s.states);
    qualities                   .update(*this, s.qualities);
    rootNotes                   .update(*this, s.rootNotes);

    for (auto p : s.progressions)
        progressions.push_back(new ChordProgression(*this, *p));

    for (auto m : s.modulations)
        modulations.push_back(new Modulation(*this, *m));
}

/**
 * Returns a string with each of the object's field values as integers. For debugging
 * @brief toString
 * @return a string representation of the object
 */
string TonalPiece::toString() const {
    string txt = "------------------------------------------------------"
                 "TonalPiece object------------------------------------------------------\n";
    txt += "Size: " + to_string(size) + "\n";
    txt += "Tonalities:\t\t";
    for(auto t : tonalities)
        txt += t->get_name() + "\t";
    txt += "\n";
    txt += "Tonalities starts:\t";
    for(auto t : tonalitiesStarts)
        txt += to_string(t) + " ";
    txt += "\n";
    txt += "Tonalities durations:\t";
    for(auto t : tonalitiesDurations)
        txt += to_string(t) + " ";
    txt += "\n";
    txt += "Modulation types:\t";
    for(auto t : modulationTypes)
        txt += to_string(t) + " ";
    txt += "\n";
    txt += "Modulation starts:\t";
    for(auto t : modulationStarts)
        txt += to_string(t) + " ";
    txt += "\n";

    txt += "States:\t\t" + intVarArray_to_string(states) + "\n";
    txt += "Qualities:\t" + intVarArray_to_string(qualities) + "\n";
    txt += "Root notes:\t" + intVarArray_to_string(rootNotes) + "\n";

    txt += "\nChord Progressions for each tonality:\n";
    for(auto p : progressions)
        txt += p->toString() + "\n\n";
    txt += "\nModulations:\n";
    for(auto m : modulations)
        txt += m->toString() + "\n\n";

    return txt;
}

string TonalPiece::pretty() const {
    string txt;
    for(int i = 0; i < progressions.size(); i++){
        txt += progressions[i]->pretty() + "\n";
        if(i < modulations.size())
            txt += modulations[i]->pretty() + "\n";
        txt += "\n";
    }
    return txt;
}

/**
 * @brief copy function
 * @return a Space* object that is a copy of the current object
 */
Space* TonalPiece::copy() {
    return new TonalPiece(*this);
}
