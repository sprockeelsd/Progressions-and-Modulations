//
// Created by Damien Sprockeels on 18/09/2024.
//

#ifndef CHORDGENERATOR_MUSICALPARTS_HPP
#define CHORDGENERATOR_MUSICALPARTS_HPP

#include "Constraints.hpp"

void tonal_progression(const Home &home, int size, int startPosition, IntVarArray &chords, IntVarArray &states,
                       IntVarArray &qualities, IntVarArray &isChromatic, IntVarArray &hasSeventh,
                       IntVarArray &bassNotes, int minChromaticChords, int maxChromaticChords, int minSeventhChords,
                       int maxSeventhChords);

#endif //CHORDGENERATOR_MUSICALPARTS_HPP
