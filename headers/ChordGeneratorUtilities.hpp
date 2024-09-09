//
// Created by Damien Sprockeels on 02/07/2024.
//

#ifndef CHORDGENERATOR_CHORDGENERATORUTILITIES_HPP
#define CHORDGENERATOR_CHORDGENERATORUTILITIES_HPP

#include "../Diatony/c++/headers/diatony/FourVoiceTexture.hpp"
#include "../Diatony/c++/headers/diatony/SolveDiatony.hpp"

///The number of supported chords, which is the size of the tonalTransitions matrix
const int nSupportedChords = 16;
///Defines possible chord successions. This enforces the following rules:
///1. The degree succession is correct
///2. Tension chords must resolve (V, V/X, VIId)
///3. Chords that can go to V can also go to Vda
///4. Chords that can go to X can also go to V/X
const IntArgs tonalTransitions = {
        //todo check this matrix with karim and maybe a teacher from the conservatoire
        //todo can x go to V/x back to x? That is a perfect cadence, implying a modulation. For now it is not allowed, but might be in the future to allow modulations
        //todo (then that should be the last thing in this tonality, then a new phrase must start in the new tonality)
        //todo check for bII and 6te aug
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
        0,     0,     0,     0,     1,     0,     0,     1,     0,     0,     0,     1,     0,     0,     0,     0,    ///bII
        0,     0,     0,     0,     1,     0,     0,     1,     0,     0,     0,     0,     0,     0,     0,     0,    ///6te_a
};

///The number of supported states, which is the size of the majorDegreeStates matrix
const int nSupportedStates = 4;
///Defines which states can be taken by chords based on their degree
//todo modify this to allow seventh chords in the future
//todo check with karim
//todo add the rules that are enforced with this
const IntArgs majorDegreeStates = {
///     fundamental state,    first inversion,   second inversion,    third inversion
                        1,                  1,                  0,                  0,    /// I
                        1,                  1,                  0,                  0,    /// II
                        1,                  0,                  0,                  0,    /// III
                        1,                  1,                  0,                  0,    /// IV
                        1,                  1,                  1,                  1,    /// V
                        1,                  0,                  0,                  0,    /// VI
                        1,                  1,                  1,                  0,    /// VII
                        0,                  0,                  1,                  0,    /// Ida
                        1,                  1,                  1,                  1,    /// V/II
                        1,                  1,                  1,                  1,    /// V/III
                        1,                  1,                  1,                  1,    /// V/IV
                        1,                  1,                  1,                  1,    /// V/V
                        1,                  1,                  1,                  1,    /// V/VI
                        1,                  1,                  1,                  1,    /// VIId
                        1,                  1,                  0,                  0,    /// bII
                        1,                  0,                  0,                  0,    /// 6te_a   todo check
};

///The number of supported qualities, which is the size of the majorDegreeQualities matrix
const int nSupportedQualities = 9;
///Defines which qualities can be taken by chords based on their degree
//todo unlock 7 chords when the basis for perfect chords is ok
const IntArgs majorDegreeQualities = {
///     M,  m,  dim,    aug,    7,  M7,     m7,     dim7,   mM7
        1,  0,    0,      0,    0,   0,      0,        0,     0,    /// I
        0,  1,    0,      0,    0,   0,      0,        0,     0,    /// II
        0,  1,    0,      0,    0,   0,      0,        0,     0,    /// III
        1,  0,    0,      0,    0,   0,      0,        0,     0,    /// IV
        1,  0,    0,      0,    1,   0,      0,        0,     0,    /// V
        0,  1,    0,      0,    0,   0,      0,        0,     0,    /// VI
        0,  0,    1,      0,    0,   0,      0,        0,     0,    /// VII
        1,  0,    0,      0,    0,   0,      0,        0,     0,    /// Ida
        1,  0,    0,      0,    1,   0,      0,        0,     0,    /// V/II
        1,  0,    0,      0,    1,   0,      0,        0,     0,    /// V/III
        1,  0,    0,      0,    1,   0,      0,        0,     0,    /// V/IV
        1,  0,    0,      0,    1,   0,      0,        0,     0,    /// V/V
        1,  0,    0,      0,    1,   0,      0,        0,     0,    /// V/VI
        0,  0,    0,      0,    0,   0,      0,        1,     0,    /// VIId
        1,  0,    0,      0,    0,   0,      0,        0,     0,    /// bII
        0,  0,    0,      1,    0,   0,      0,        0,     0,    /// 6te_a   todo make this correct, this is not right



};


int* IntVarArray_to_int_pointer(IntVarArray vars);

vector<int> IntVarArray_to_int_vector(IntVarArray vars);

#endif //CHORDGENERATOR_CHORDGENERATORUTILITIES_HPP
