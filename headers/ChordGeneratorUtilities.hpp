//
// Created by Damien Sprockeels on 02/07/2024.
//

#ifndef CHORDGENERATOR_CHORDGENERATORUTILITIES_HPP
#define CHORDGENERATOR_CHORDGENERATORUTILITIES_HPP

#include "../Diatony/c++/headers/diatony/FourVoiceTexture.hpp"
#include "../Diatony/c++/headers/diatony/SolveDiatony.hpp"

const IntArgs majorTonalityTransitions = {
///     I,      II,     III,    IV,     V,      VI,     VII
        1,      1,      1,      1,      1,      1,      1,    /// I
        1,      1,      0,      1,      1,      0,      0,    /// II
        1,      0,      0,      0,      0,      1,      0,    /// III
        1,      1,      0,      1,      1,      1,      1,    /// IV
        1,      0,      0,      1,      1,      1,      0,    /// V
        1,      1,      0,      1,      1,      0,      0,    /// VI
        1,      0,      1,      0,      0,      0,      0     /// VII
};

enum borrowedChords{
    DIATONIC,   //0
    BORROWED,   //1
};

int* IntVarArray_to_int_pointer(IntVarArray vars);

vector<int> IntVarArray_to_int_vector(IntVarArray vars);

#endif //CHORDGENERATOR_CHORDGENERATORUTILITIES_HPP
