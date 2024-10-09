//
// Created by Damien Sprockeels on 07/10/2024.
//

#ifndef CHORDGENERATOR_MODULATION_HPP
#define CHORDGENERATOR_MODULATION_HPP

#include "ChordGeneratorUtilities.hpp"
#include "ChordProgression.hpp"


class Modulation {
private:
    int type;
    int start;
    int end;

    ChordProgression* from;
    ChordProgression* to;

public:
    Modulation(const Home &home, int type, int start, int end, ChordProgression *from, ChordProgression *to);

    Modulation(const Home &home, const Modulation& m);

    string toString();
};

#endif //CHORDGENERATOR_MODULATION_HPP
