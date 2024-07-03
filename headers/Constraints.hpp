//
// Created by Damien Sprockeels on 03/07/2024.
//

#ifndef CHORDGENERATOR_CONSTRAINTS_HPP
#define CHORDGENERATOR_CONSTRAINTS_HPP

#include "ChordGeneratorUtilities.hpp"

/**
 * Posts the constraint that the fifth degree appogiatura (V64-I in second inversion) must resolve to the fifth degree
 * chord in fundamental state (V5-V7+)
 * @param home the problem space
 * @param curPos the current position in the chord progression
 * @param chords the variables for the chords of the progression
 * @param states the variables for the state of the chords
 */
void fifth_degree_appogiatura(const Home& home, int curPos, IntVarArray chords, IntVarArray states);


/**
 * Post the constraint that borrowed chords must resolve. That is, they must be followed by a chord based on the relative "tonic".
 * For example, the V/III chord must be followed by a chord based on the III (either the III or the V/VI chord).
 * @param home the problem space
 * @param curPos the current position in the chord progression
 * @param chords the variables for the chords of the progression
 * @param borrowedChords the variables saying whether the chord is borrowed
 */
void borrowed_chords_resolution(const Home& home, int curPos, IntVarArray chords, IntVarArray borrowedChords);

#endif //CHORDGENERATOR_CONSTRAINTS_HPP
