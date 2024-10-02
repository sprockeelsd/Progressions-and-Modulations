//
// Created by Damien Sprockeels on 30/09/2024.
//

#include <utility>

#include "../headers/TonalPiece.hpp"

TonalPiece::TonalPiece(int size, const vector<Tonality *>& tonalities, vector<int> tonalitiesStarts,
           vector<int> tonalitiesDurations, vector<int> modulationTypes,
           vector<int> modulationStarts) {
    ///parameters
    this->size = size;
    this->tonalities = tonalities;
    this->tonalitiesStarts = tonalitiesStarts;
    this->tonalitiesDurations = tonalitiesDurations;
    this->modulationTypes = modulationTypes;
    this->modulationStarts = modulationStarts;

    this->states = IntVarArray(*this, size, FUNDAMENTAL_STATE, THIRD_INVERSION);
    this->qualities = IntVarArray(*this, size, MAJOR_CHORD, MINOR_MAJOR_SEVENTH_CHORD);
    this->rootNotes = IntVarArray(*this, size, C, B);

    progressions.reserve(tonalities.size());
    for (int i = 0; i < tonalities.size(); i++){
        progressions.push_back(new ChordProgression(*this, tonalities[i], tonalitiesStarts[i], tonalitiesDurations[i],
                                                    states, qualities, rootNotes, 0,
                                                    1, 0, 1));
    }

    //todo branch on degrees in each tonality, not on these
//    branch(*this, states, INT_VAR_SIZE_MIN(), INT_VAL_MIN());
    branch(*this, qualities, INT_VAR_SIZE_MIN(), INT_VAL_MIN());
//    branch(*this, rootNotes, INT_VAR_SIZE_MIN(), INT_VAL_MIN());
    //todo move sevenths here because it is not tonality dependent
}

string TonalPiece::toString() const {
    string txt;
    txt += "Size: " + to_string(size) + "\n";
    txt += "Tonalities: ";
    for(auto t : tonalities)
        txt += t->get_name() + " ";
    txt += "\n";
    txt += "Tonalities starts: ";
    for(auto t : tonalitiesStarts)
        txt += to_string(t) + " ";
    txt += "\n";
    txt += "Tonalities durations: ";
    for(auto t : tonalitiesDurations)
        txt += to_string(t) + " ";
    txt += "\n";
    txt += "Modulation types: ";
    for(auto t : modulationTypes)
        txt += to_string(t) + " ";
    txt += "\n";
    txt += "Modulation starts: ";
    for(auto t : modulationStarts)
        txt += to_string(t) + " ";
    txt += "\n";

    txt += "States: " + intVarArray_to_string(states) + "\n";
    txt += "Qualities: " + intVarArray_to_string(qualities) + "\n";
    txt += "Root notes: " + intVarArray_to_string(rootNotes) + "\n";

    txt += "Progressions: \n";
    for(auto p : progressions)
        txt += p->toString() + "\n\n";

    return txt;
}

/**
 * @brief ChordProgression
 * @param s a ChordProgression object
 */
TonalPiece::TonalPiece(TonalPiece &s) : Space(s){
    size                    = s.size;
    tonalities              = s.tonalities;
    tonalitiesStarts       = s.tonalitiesStarts;
    tonalitiesDurations    = s.tonalitiesDurations;
    modulationTypes         = s.modulationTypes;
    modulationStarts        = s.modulationStarts;
    states                  .update(*this, s.states);
    qualities               .update(*this, s.qualities);
    rootNotes               .update(*this, s.rootNotes);
    for (auto p : s.progressions)
        progressions.push_back(new ChordProgression(*this, *p));
}

/**
 * @brief copy
 * @return a Space* object that is a copy of the current object
 */
Space* TonalPiece::copy() {
    return new TonalPiece(*this);
}
