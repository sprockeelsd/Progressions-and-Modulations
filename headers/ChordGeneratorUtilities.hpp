//
// Created by Damien Sprockeels on 02/07/2024.
//

#ifndef CHORDGENERATOR_CHORDGENERATORUTILITIES_HPP
#define CHORDGENERATOR_CHORDGENERATORUTILITIES_HPP

#include "../Diatony/c++/headers/diatony/FourVoiceTexture.hpp"
#include "../Diatony/c++/headers/diatony/SolveDiatony.hpp"

//todo move all this to Tonality classes

///The number of supported chords, which is the size of the tonalTransitions matrix
constexpr int nSupportedChords = 16;
///Defines possible chord successions.
const IntArgs tonalTransitions = {
///     I,    II,   III,    IV,     V,    VI,   VII,   Vda,  V/II, V/III,  V/IV,   V/V,  V/VI, V/VII,   bII, 6te_a
        1,     1,     1,     1,     1,     1,     1,     0,     1,     1,     1,     1,     1,     1,     1,     1,    /// I
        1,     1,     0,     1,     1,     0,     0,     1,     0,     0,     1,     1,     0,     0,     0,     0,    /// II
        0,     0,     0,     0,     0,     1,     0,     0,     0,     0,     0,     0,     1,     0,     0,     0,    /// III
        1,     1,     0,     1,     1,     0,     1,     1,     1,     0,     0,     1,     0,     1,     1,     1,    /// IV
        1,     0,     0,     1,     1,     1,     0,     0,     0,     0,     1,     0,     1,     0,     0,     0,    /// V
        0,     1,     0,     1,     1,     0,     0,     1,     1,     0,     1,     1,     0,     0,     1,     1,    /// VI
        1,     0,     1,     0,     0,     0,     0,     0,     0,     1,     0,     0,     0,     0,     0,     0,    /// VII
        0,     0,     0,     0,     1,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,    ///Vda
        0,     1,     0,     0,     0,     0,     0,     0,     0,     0,     0,     1,     0,     0,     0,     0,    ///V/II
        0,     0,     1,     0,     0,     0,     0,     0,     0,     0,     0,     0,     1,     0,     0,     0,    ///V/III
        0,     0,     0,     1,     0,     0,     0,     0,     0,     0,     0,     0,     0,     1,     0,     0,    ///V/IV
        0,     0,     0,     0,     1,     0,     0,     1,     0,     0,     0,     0,     0,     0,     0,     0,    ///V/V
        0,     0,     0,     0,     0,     1,     0,     0,     1,     0,     0,     0,     0,     0,     0,     0,    ///V/VI
        0,     0,     0,     0,     0,     0,     1,     0,     0,     1,     0,     0,     0,     0,     0,     0,    ///V/VII
        0,     0,     0,     0,     1,     0,     0,     1,     0,     0,     0,     0,     0,     0,     0,     0,    ///bII
        0,     0,     0,     0,     1,     0,     0,     1,     0,     0,     0,     0,     0,     0,     0,     0,    ///6te_a
};

///The number of supported states, which is the size of the degreeStates matrix
constexpr int nSupportedStates = 5;
///Defines which states can be taken by chords based on their degree
const IntArgs degreeStates = {
///     fundamental state,    first inversion,   second inversion,    third inversion,      fourth inversion
                        1,                  1,                  0,                  0,                     0,    /// I
                        1,                  1,                  0,                  0,                     0,    /// II
                        1,                  0,                  0,                  0,                     0,    /// III
                        1,                  1,                  0,                  0,                     0,    /// IV
                        1,                  1,                  1,                  1,                     0,    /// V
                        1,                  0,                  0,                  0,                     0,    /// VI
                        1,                  1,                  1,                  0,                     0,    /// VII
                        0,                  0,                  1,                  0,                     0,    /// Vda
                        1,                  1,                  1,                  1,                     0,    /// V/II
                        1,                  1,                  1,                  1,                     0,    /// V/III
                        1,                  1,                  1,                  1,                     0,    /// V/IV
                        1,                  1,                  1,                  1,                     0,    /// V/V
                        1,                  1,                  1,                  1,                     0,    /// V/VI
                        1,                  1,                  1,                  1,                     0,    /// V/VII
                        1,                  1,                  0,                  0,                     0,    /// bII
                        1,                  0,                  0,                  0,                     0,    /// 6te_a   todo maybe allow for more states later
};

///The number of supported qualities, which is the size of the majorDegreeQualities matrix
constexpr int nSupportedQualities = 13;
///Defines which qualities can be taken by chords based on their degree
//todo add alternative chords like mIV in major or mV in minor
//todo add 9th etc
const IntArgs majorDegreeQualities = {
///     M,  m,  dim,    aug, Augmented Sixth,   7,  M7,     m7,     dim7, half dim,    mM7,   major_ninth_dom, minor_ninth_dom
        1,  0,    0,      0,               0,   0,   1,      0,        0,        0,      0,                 0,              0,    /// I
        0,  1,    0,      0,               0,   0,   0,      1,        0,        0,      0,                 0,              0,    /// II
        0,  1,    0,      0,               0,   0,   0,      1,        0,        0,      0,                 0,              0,    /// III
        1,  0,    0,      0,               0,   0,   1,      0,        0,        0,      0,                 0,              0,    /// IV
        1,  0,    0,      0,               0,   1,   0,      0,        1,        0,      0,                 1,              1,    /// V
        0,  1,    0,      0,               0,   0,   0,      1,        0,        0,      0,                 0,              0,    /// VI
        0,  0,    1,      0,               0,   0,   0,      0,        0,        1,      0,                 0,              0,    /// VII
        1,  0,    0,      0,               0,   0,   0,      0,        0,        0,      0,                 0,              0,    /// Vda
        1,  0,    0,      0,               0,   1,   0,      0,        1,        0,      0,                 1,              1,    /// V/II
        1,  0,    0,      0,               0,   1,   0,      0,        1,        0,      0,                 1,              1,    /// V/III
        1,  0,    0,      0,               0,   1,   0,      0,        1,        0,      0,                 1,              1,    /// V/IV
        1,  0,    0,      0,               0,   1,   0,      0,        1,        0,      0,                 1,              1,    /// V/V
        1,  0,    0,      0,               0,   1,   0,      0,        1,        0,      0,                 1,              1,    /// V/VI
        1,  0,    0,      0,               0,   1,   0,      0,        1,        0,      0,                 1,              1,    /// V/VII
        1,  0,    0,      0,               0,   0,   0,      0,        0,        0,      0,                 0,              0,    /// bII
        0,  0,    0,      0,               1,   0,   0,      0,        0,        0,      0,                 0,              0,    /// 6te_a
};

const IntArgs minorDegreeQualities = {
///     M,  m,  dim,    aug, Augmented Sixth,   7,  M7,     m7,     dim7, half dim,    mM7,  major_ninth_dom, minor_ninth_dom
        0,  1,    0,      0,               0,   0,   0,      1,        0,        0,     0,                 0,              0,    /// I
        0,  0,    1,      0,               0,   0,   0,      0,        0,        1,     0,                 0,              0,    /// II
        1,  0,    0,      1,               0,   0,   1,      0,        0,        0,     0,                 0,              0,    /// III
        0,  1,    0,      0,               0,   0,   0,      1,        0,        0,     0,                 0,              0,    /// IV
        1,  1,    0,      0,               0,   1,   0,      0,        1,        0,     0,                 1,              1,    /// V
        1,  0,    0,      0,               0,   0,   1,      0,        0,        0,     0,                 0,              0,    /// VI
        1,  0,    1,      0,               0,   0,   0,      0,        1,        0,     0,                 0,              0,    /// VII
        0,  1,    0,      0,               0,   0,   0,      0,        0,        0,     0,                 0,              0,    /// Vda
        1,  0,    0,      0,               0,   1,   0,      0,        1,        0,     0,                 1,              1,    /// V/II
        1,  0,    0,      0,               0,   1,   0,      0,        1,        0,     0,                 1,              1,    /// V/III
        1,  0,    0,      0,               0,   1,   0,      0,        1,        0,     0,                 1,              1,    /// V/IV
        1,  0,    0,      0,               0,   1,   0,      0,        1,        0,     0,                 1,              1,    /// V/V
        1,  0,    0,      0,               0,   1,   0,      0,        1,        0,     0,                 1,              1,    /// V/VI
        1,  0,    0,      0,               0,   1,   0,      0,        1,        0,     0,                 1,              1,    /// V/VII
        1,  0,    0,      0,               0,   0,   0,      0,        0,        0,     0,                 0,              0,    /// bII
        0,  0,    0,      0,               1,   0,   0,      0,        0,        0,     0,                 0,              0,    /// 6te_a
};

const IntArgs bassBasedOnDegreeAndState = {
///                  root,              third,              fifth,            seventh,                nineth
///     fundamental state,    first inversion,   second inversion,    third inversion,      fourth inversion
             FIRST_DEGREE,       THIRD_DEGREE,       FIFTH_DEGREE,     SEVENTH_DEGREE,         SECOND_DEGREE,         /// I
            SECOND_DEGREE,      FOURTH_DEGREE,       SIXTH_DEGREE,       FIRST_DEGREE,          THIRD_DEGREE,         /// II
             THIRD_DEGREE,       FIFTH_DEGREE,     SEVENTH_DEGREE,      SECOND_DEGREE,         FOURTH_DEGREE,         /// III
            FOURTH_DEGREE,       SIXTH_DEGREE,       FIRST_DEGREE,       THIRD_DEGREE,          FIFTH_DEGREE,         /// IV
             FIFTH_DEGREE,     SEVENTH_DEGREE,      SECOND_DEGREE,      FOURTH_DEGREE,          SIXTH_DEGREE,         /// V
             SIXTH_DEGREE,       FIRST_DEGREE,       THIRD_DEGREE,       FIFTH_DEGREE,        SEVENTH_DEGREE,         /// VI
           SEVENTH_DEGREE,      SECOND_DEGREE,      FOURTH_DEGREE,       SIXTH_DEGREE,          FIRST_DEGREE,         /// VII
             FIRST_DEGREE,       THIRD_DEGREE,       FIFTH_DEGREE,     SEVENTH_DEGREE,         SECOND_DEGREE,         /// Vda
             SIXTH_DEGREE,       FIRST_DEGREE,       THIRD_DEGREE,       FIFTH_DEGREE,        SEVENTH_DEGREE,         /// V/II
           SEVENTH_DEGREE,      SECOND_DEGREE,      FOURTH_DEGREE,       SIXTH_DEGREE,          FIRST_DEGREE,         /// V/III
             FIRST_DEGREE,       THIRD_DEGREE,       FIFTH_DEGREE,     SEVENTH_DEGREE,         SECOND_DEGREE,         /// V/IV
            SECOND_DEGREE,      FOURTH_DEGREE,       SIXTH_DEGREE,       FIRST_DEGREE,          THIRD_DEGREE,         /// V/V
             THIRD_DEGREE,       FIFTH_DEGREE,     SEVENTH_DEGREE,      SECOND_DEGREE,         FOURTH_DEGREE,         /// V/VI
            FOURTH_DEGREE,       SIXTH_DEGREE,       FIRST_DEGREE,       THIRD_DEGREE,          FIFTH_DEGREE,         /// V/VII
            SECOND_DEGREE,      FOURTH_DEGREE,       SIXTH_DEGREE,       FIRST_DEGREE,          THIRD_DEGREE,         /// bII
             SIXTH_DEGREE,       FIRST_DEGREE,       THIRD_DEGREE,      FOURTH_DEGREE,         SECOND_DEGREE,         /// 6te_a
};

//todo move these functions to the Utility class

/**
 * Converts an IntVarArray to a pointer to an array of integers.
 * @param vars The IntVarArray to convert.
 * @return A pointer to an array of integers representing the values in the IntVarArray.
 */
int* IntVarArray_to_int_pointer(IntVarArray vars);

/**
 * Converts an IntVarArray to a vector of integers.
 * @param vars The IntVarArray to convert.
 * @return A vector of integers representing the values in the IntVarArray.
 */
 vector<int> IntVarArray_to_int_vector(const IntVarArray& vars);

#endif //CHORDGENERATOR_CHORDGENERATORUTILITIES_HPP