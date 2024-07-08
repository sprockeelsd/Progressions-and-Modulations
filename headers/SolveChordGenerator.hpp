//
// Created by Damien Sprockeels on 05/07/2024.
//

#ifndef CHORDGENERATOR_SOLVE_CHORD_GENERATOR_HPP
#define CHORDGENERATOR_SOLVE_CHORD_GENERATOR_HPP

#include "ChordGeneratorUtilities.hpp"
#include "ChordGenerator.hpp"

vector<ChordGenerator*> solve_chord_progression_problem(int size, Tonality* tonality);

ChordGenerator* solve_chord_progression_problem_best(int size, Tonality* tonality);

#endif //CHORDGENERATOR_SOLVE_CHORD_GENERATOR_HPP
