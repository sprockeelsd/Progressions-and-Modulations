//
// Created by Damien Sprockeels on 02/07/2024.
//

#ifndef CHORDGENERATOR_CHORDGENERATOR_HPP
#define CHORDGENERATOR_CHORDGENERATOR_HPP

#include "ChordGeneratorUtilities.hpp"
#include "Constraints.hpp"

class ChordGenerator : public Space{
private:
    int size;
    Tonality* tonality;
    IntVarArray chords;
    IntVarArray states;
    IntVarArray qualities;

public:
    ChordGenerator(int size, Tonality* tonality);
    ChordGenerator(ChordGenerator &s);
    virtual Space* copy();

    int getSize() const { return size; }
    Tonality* getTonality() const { return tonality; }
    IntVarArray getChords() const { return chords; }
    IntVarArray getStates() const { return states; }

    string toString() const;

    string pretty() const;

};


#endif //CHORDGENERATOR_CHORDGENERATOR_HPP
