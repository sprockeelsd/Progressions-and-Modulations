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
    Modulation(Home home, int type, int start, int end, ChordProgression *from, ChordProgression *to);

    Modulation(const Home &home, const Modulation& m);

    void perfect_cadence_modulation(const Home &home);

    void pivot_chord_modulation(const Home &home);

    void alteration_modulation(Home home);

    void secondary_dominant_modulation(Home home);

    string toString();

    string pretty();
};

#endif //CHORDGENERATOR_MODULATION_HPP
