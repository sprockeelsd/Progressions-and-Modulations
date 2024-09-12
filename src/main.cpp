//
// Created by Damien Sprockeels on 02/07/2024.
//

#include "../headers/ChordGeneratorUtilities.hpp"
#include "../headers/ChordGenerator.hpp"
#include "../headers/SolveChordGenerator.hpp"
#include "../Diatony/c++/headers/aux/MidiFileGeneration.hpp"

int main(int argc, char **argv) {
    string four_voice = argv[1]; /// true if we want to generate the 4voice chords, false if we just want chords and state
    int size = 4;
    Tonality* Cmajor = new MajorTonality(C);

    auto sol = solve_chord_progression_problem_best(size, Cmajor);
    auto chords = IntVarArray_to_int_vector(sol->getChords());
    auto states = IntVarArray_to_int_vector(sol->getStates());
    vector<int> qualities;
    qualities.reserve(chords.size());
    for(int chord : chords)
        qualities.push_back(Cmajor->get_chord_quality(chord));

    if(four_voice == "true"){
        auto diatony_sol = solve_diatony_problem_optimal(4, Cmajor, chords, qualities, states);

        writeSolToMIDIFile(size, "output", diatony_sol);

        std::cout << "MIDI file created." << std::endl;
        std::cout << "\n\n-----------------------------------------------------------------------------------------------------" <<
                  "\n\nSummary:\n\n" <<  "-----------------------------------------------------------------------------------------------------\n\n" <<
                  sol->pretty() << std::endl << "Solution:\n" << diatony_sol->to_string() << std::endl;
    }
    return 0;
}
