//
// Created by Damien Sprockeels on 02/07/2024.
//

#ifndef CHORDGENERATOR_CHORDGENERATORUTILITIES_HPP
#define CHORDGENERATOR_CHORDGENERATORUTILITIES_HPP

#include "../Diatony/c++/headers/diatony/FourVoiceTexture.hpp"
#include "../Diatony/c++/headers/diatony/SolveDiatony.hpp"

const IntArgs majorTonalityTransitions = {
        //todo check this matrix with karim and maybe a teacher from the conservatoire
        //todo can x go to V/x back to x? That is a perfect cadence, implying a modulation. For now it is not allowed, but might be in the future to allow modulations
        //todo (then that should be the last thing in this tonality, then a new phrase must start in the new tonality)
///     I,    II,   III,    IV,     V,    VI,   VII,   Ida,  V/II, V/III,  V/IV,   V/V,  V/VI,  VIId,   bII, 6te_a
        1,     1,     1,     1,     1,     1,     1,     0,     1,     1,     1,     1,     1,     1,     1,     1,    /// I
        1,     1,     0,     1,     1,     0,     0,     1,     0,     0,     1,     1,     0,     1,     0,     0,    /// II
        1,     0,     0,     0,     0,     1,     0,     0,     0,     0,     0,     0,     1,     0,     0,     0,    /// III
        1,     1,     0,     1,     1,     1,     1,     1,     1,     0,     0,     1,     1,     1,     1,     1,    /// IV
        1,     0,     0,     1,     1,     1,     0,     0,     0,     0,     1,     0,     1,     0,     0,     0,    /// V
        1,     1,     0,     1,     1,     0,     0,     1,     1,     0,     1,     1,     0,     0,     1,     0,    /// VI
        1,     0,     1,     0,     0,     0,     0,     0,     0,     1,     0,     0,     0,     1,     0,     0,    /// VII
        0,     0,     0,     0,     1,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,    ///Ida
        0,     1,     0,     0,     0,     0,     0,     0,     0,     0,     0,     1,     0,     0,     0,     0,    ///V/II
        0,     0,     1,     0,     0,     0,     0,     0,     0,     0,     0,     0,     1,     0,     0,     0,    ///V/III
        0,     0,     0,     1,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,    ///V/IV
        0,     0,     0,     0,     1,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,    ///V/V
        0,     0,     0,     0,     0,     1,     0,     0,     1,     0,     0,     0,     0,     0,     0,     0,    ///V/VI
        1,     0,     0,     0,     1,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,    ///VIId
        0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,    ///bII
        0,     0,     0,     0,     1,     0,     0,     1,     0,     0,     0,     0,     0,     0,     0,     0,    ///6te_a
};

int* IntVarArray_to_int_pointer(IntVarArray vars);

vector<int> IntVarArray_to_int_vector(IntVarArray vars);

#endif //CHORDGENERATOR_CHORDGENERATORUTILITIES_HPP
