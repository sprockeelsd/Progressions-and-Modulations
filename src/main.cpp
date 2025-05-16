//
// Created by Damien Sprockeels on 02/07/2024.
//

#include "../headers/ChordGeneratorUtilities.hpp"
#include "../Diatony/c++/headers/aux/MidiFileGeneration.hpp"

#include "../headers/HarmoniserSolver.hpp"

int main(int argc, char **argv) {
    string four_voice = argv[1]; /// true if we want to generate the 4voice chords, false if we just want chords and state

    int size = 20;
    Tonality* Cminor = new MinorTonality(C);    Tonality* Ebmajor = new MajorTonality(E_FLAT);
    vector<Tonality*> tonalities = {Cminor, Ebmajor};
    vector<int> modulationTypes = {PERFECT_CADENCE_MODULATION};
    vector<int> modulationStarts = {12};
    vector<int> modulationEnds = {13};

    auto params = TonalPieceParameters(size, tonalities.size(), tonalities,
                                       modulationTypes, modulationStarts, modulationEnds);

    //vector<int> phrase_starts = {0, 14};

    auto tonalPiece = new TonalPiece(&params);

    const auto sol = solve_harmoniser(tonalPiece);
    std::cout << "Best solution: \n" << sol->pretty() << std::endl;

    return 0;
}