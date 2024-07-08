//
// Created by Damien Sprockeels on 02/07/2024.
//

#include "../headers/ChordGeneratorUtilities.hpp"
#include "../headers/ChordGenerator.hpp"
#include "../headers/SolveChordGenerator.hpp"
#include "../Diatony/c++/headers/aux/MidiFileGeneration.hpp"

int main(int argc, char **argv) {
    Tonality* Gmajor = new MajorTonality(G);

    auto sol = solve_chord_progression_problem_best(4, Gmajor);
    auto chords = IntVarArray_to_int_vector(sol->getChords());
    auto states = IntVarArray_to_int_vector(sol->getStates());
    vector<int> qualities;
    qualities.reserve(chords.size());
    for(int i = 0; i < chords.size(); i++)
        qualities.push_back(Gmajor->get_chord_quality(chords[i]));

    std::cout << "chords" << int_vector_to_string(chords) << std::endl;
    std::cout << sol->getChords() << std::endl;


    auto diatony_sol = solve_diatony_problem_optimal(4, Gmajor, chords, qualities, states);

    writeSolToMIDIFile(4, "output", diatony_sol);
    
    std::cout << "MIDI file created." << std::endl;
    std::cout << "\n\n-----------------------------------------------------------------------------------------------------" <<
    "Summary:\n\n" <<  "-----------------------------------------------------------------------------------------------------\n\n" <<
    "Chord progression:\n" << sol->pretty() << std::endl << "Solution:\n" << diatony_sol->to_string() << std::endl;

    return 0;
}
