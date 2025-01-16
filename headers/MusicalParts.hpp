//
// Created by Damien Sprockeels on 18/09/2024.
//

#ifndef CHORDGENERATOR_MUSICALPARTS_HPP
#define CHORDGENERATOR_MUSICALPARTS_HPP

#include "Constraints.hpp"

/**
 * Posts all the constraints that a tonal chord progression must respect in a given tonality.
 * @param home the problem space
 * @param size the number of chords in the progression
 * @param tonality the tonality of the progression
 * @param states the states of the chords
 * @param qualities the qualities of the chords
 * @param rootNotes the root notes of the chords
 * @param chords the degrees of the chords
 * @param bassDegrees the degree of the note at the bass
 * @param isChromatic whether each chord is chromatic or not
 * @param hasSeventh whether each chord has a seventh or not
 * @param roots the root notes of the chords
 * @param thirds the third notes of the chords
 * @param fifths the fifth notes of the chords
 * @param sevenths the seventh notes of the chords
 * @param minChromaticChords the minimum number of chromatic chords in the progression
 * @param maxChromaticChords the maximum number of chromatic chords in the progression
 * @param minSeventhChords the minimum number of seventh chords in the progression
 * @param maxSeventhChords the maximum number of seventh chords in the progression
 */
void tonal_progression(Home home, int size, Tonality *tonality, IntVarArray &states, IntVarArray &qualities,
                       IntVarArray &rootNotes, IntVarArray &chords, IntVarArray &bassDegrees, IntVarArray &isChromatic,
                       IntVarArray &hasSeventh, const IntVarArray& roots, const IntVarArray& thirds, const IntVarArray& fifths,
                       const IntVarArray& sevenths, int minChromaticChords, int maxChromaticChords, int minSeventhChords,
                       int maxSeventhChords);

#endif //CHORDGENERATOR_MUSICALPARTS_HPP
