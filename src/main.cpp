//
// Created by Damien Sprockeels on 02/07/2024.
//

#include "../headers/ChordGeneratorUtilities.hpp"
#include "../headers/ChordGenerator.hpp"
#include "../headers/SolveChordGenerator.hpp"

int main(int argc, char **argv) {
    Tonality* Gmajor = new MajorTonality(G);

    auto sols = solve_chord_progression_problem(4, Gmajor);
    auto sol = sols.back();
    auto chords = IntVarArray_to_int_vector(sol->getChords());
    auto states = IntVarArray_to_int_vector(sol->getStates());
    vector<int> qualities;
    qualities.reserve(chords.size());
    for(int i = 0; i < chords.size(); i++)
        qualities.push_back(Gmajor->get_chord_quality(chords[i]));

    auto diatony_sols = solve_diatony_problem(4, Gmajor, chords, qualities, states); //todo segfault

    return 0;
}
