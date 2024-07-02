//
// Created by Damien Sprockeels on 02/07/2024.
//

#ifndef CHORDGENERATOR_CHORDGENERATOR_HPP
#define CHORDGENERATOR_CHORDGENERATOR_HPP

#include "ChordGeneratorUtilities.hpp"

class ChordGenerator : public Space{
private:
    int size;
    IntVarArray chords;

public:
    ChordGenerator(int size);
    ChordGenerator(ChordGenerator &s);
    virtual Space* copy();

    IntVarArray getChords() { return chords; }
};


#endif //CHORDGENERATOR_CHORDGENERATOR_HPP
