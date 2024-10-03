//
// Created by Damien Sprockeels on 02/07/2024.
//

#include "../headers/ChordGeneratorUtilities.hpp"
#include "../headers/ChordProgression.hpp"
#include "../headers/SolveChordGenerator.hpp"
#include "../Diatony/c++/headers/aux/MidiFileGeneration.hpp"
#include "../headers/TonalPiece.hpp"

int main(int argc, char **argv) {
    string four_voice = argv[1]; /// true if we want to generate the 4voice chords, false if we just want chords and state
    Tonality* Cmajor = new MajorTonality(C);
    Tonality* Gmajor = new MajorTonality(G);
    vector<Tonality*> tonalities = {Cmajor, Gmajor};
    vector<int> tonalitiesStarts = {0, 9};
    vector<int> tonalitiesDurations = {10, 11};
    vector<int> modulationTypes = {PERFECT_CADENCE_MODULATION};
    vector<int> modulationStarts = {8};
    int size = tonalitiesStarts[tonalitiesStarts.size()-1] + tonalitiesDurations[tonalitiesDurations.size()-1];

    auto tonalPiece = new TonalPiece(size, tonalities, tonalitiesStarts, tonalitiesDurations, modulationTypes, modulationStarts);
    std::cout << tonalPiece->toString() << std::endl;

    DFS<TonalPiece> engine(tonalPiece);
    delete tonalPiece;

    int n_sols = 0;
    while(TonalPiece* sol = engine.next()) {
        n_sols += 1;
        std::cout << "Solution:\n" << sol->toString() << std::endl;
        if(n_sols >= 10) break;
        delete sol;
    }
    if (n_sols == 0)
        std::cout << "No solution found." << std::endl;
    else
        std::cout << "Number of solutions: " << n_sols << std::endl;

//    auto sol   = solve_chord_progression_problem_best(size, tonalities, tonalitiesStarts, tonalitiesDurations,
//                                                      modulationTypes, modulationStarts);
//
//    auto chords     = IntVarArray_to_int_vector(sol->getChords());
//    auto states     = IntVarArray_to_int_vector(sol->getStates());
//    auto qualities  = IntVarArray_to_int_vector(sol->getQualities());
//
//    if(four_voice == "true"){
//        std::cout << "Chords: "     << int_vector_to_string(chords)     << std::endl;
//        std::cout << "States: "     << int_vector_to_string(states)     << std::endl;
//        std::cout << "Qualities: "  << int_vector_to_string(qualities)  << std::endl;
//        //todo split the output from ChordProgression based on the tonalities and the modulations
//
//        auto diatony_sol = solve_diatony_problem_optimal(size, Cmajor, chords, qualities, states);
//
//        writeSolToMIDIFile(size, "output", diatony_sol);
//
//        std::cout << "MIDI file created." << std::endl;
//        std::cout << "\n\n-----------------------------------------------------------------------------------------------------" <<
//                  "\n\n \t\t\t\t\tSummary:\n\n" <<  "-----------------------------------------------------------------------------------------------------\n\n" <<
//                  sol->pretty() << std::endl << "Solution:\n" << diatony_sol->to_string() << std::endl;
//    }
    return 0;
}
