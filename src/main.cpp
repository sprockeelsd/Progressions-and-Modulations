//
// Created by Damien Sprockeels on 02/07/2024.
//

#include "../headers/ChordGeneratorUtilities.hpp"
#include "../Diatony/c++/headers/aux/MidiFileGeneration.hpp"

#include "../headers/HarmoniserSolver.hpp"

// todo ajouter les 64 de passage (cst en plus du coup)
int main(int argc, char **argv) {
    string four_voice = argv[1]; /// true if we want to generate the 4voice chords, false if we just want chords and state

    // parameters of the layer 2 problem
    int size = 20;
    Tonality* Cminor = new MinorTonality(C);    Tonality* Ebmajor = new MajorTonality(E_FLAT);
    vector<Tonality*> tonalities = {Cminor, Ebmajor};
    vector<int> modulationTypes = {SECONDARY_DOMINANT_MODULATION};
    vector<int> modulationStarts = {12};
    vector<int> modulationEnds = {13};

    auto params = TonalPieceParameters(size, static_cast<int>(tonalities.size()), tonalities,
                                       modulationTypes, modulationStarts, modulationEnds);

    // Create an instance of the layer 2 problem (progressions and modulations)
    auto tonalPiece = new TonalPiece(&params);

    // Solve layer 2 problem
    const auto sol = solve_harmoniser(tonalPiece);
    std::cout << "Best solution: \n" << sol->pretty() << std::endl;

    // Create the parameters for the voicing problem
    vector<TonalProgressionParameters*> sectionParams;
    vector<ModulationParameters*> modulationParams;
    sectionParams.reserve(tonalities.size());
    modulationParams.reserve(tonalities.size() - 1);

    // create the section parameters for diatony
    for (int i = 0; i < tonalities.size(); i++) {
        vector<int> chord_degrees = intVarArray_to_int_vector(sol->getChordProgression(i)->getChords());
        vector<int> chord_qualities = intVarArray_to_int_vector(sol->getChordProgression(i)->getQualities());
        vector<int> chord_states = intVarArray_to_int_vector(sol->getChordProgression(i)->getStates());

        const auto sec_params = new TonalProgressionParameters(i, sol->getChordProgression(i)->getDuration(),
                                                         sol->getChordProgression(i)->getStart(),
                                                         sol->getChordProgression(i)->getStart() + sol->getChordProgression(i)->getDuration() - 1,
                                                         tonalities[i],
                                                         chord_degrees,
                                                         chord_qualities,
                                                         chord_states);
        sectionParams.push_back(new TonalProgressionParameters(sec_params));
        delete sec_params;
    }

    // create the modulation parameters
    for (int i = 0; i < modulationTypes.size(); i++) {
        auto mod_params = new ModulationParameters(modulationTypes[i], modulationStarts[i], modulationEnds[i],
                                                    sectionParams[i], sectionParams[i+1]);
        modulationParams.push_back(new ModulationParameters(mod_params));
        delete mod_params;
    }

    // create the parameters for the four voice texture problem
    auto pieceParams = new FourVoiceTextureParameters(size, static_cast<int>(tonalities.size()), sectionParams, modulationParams);

    // Search options
    Options opts;
    opts.threads = 1;
    opts.stop = Stop::time(60000); // stop after 120 seconds
    opts.cutoff = Cutoff::merge(
            Cutoff::linear(2*pieceParams->get_totalNumberOfChords()),
            Cutoff::geometric((4*pieceParams->get_totalNumberOfChords())^2, 2));
    opts.nogoods_limit = pieceParams->get_totalNumberOfChords() * 4 * 4;

    // solve the problem and measure the time taken
    auto start = std::chrono::high_resolution_clock::now();     /// start time
    auto best_sol = solve_diatony(pieceParams, &opts, true);
    auto currTime = std::chrono::high_resolution_clock::now();     /// current time
    std::chrono::duration<double> duration = currTime - start; // elapsed time

    std::cout << "search over" << std::endl;

    if(best_sol != nullptr){
        writeSolToMIDIFile(best_sol->getParameters()->get_totalNumberOfChords(), "out/MidiFiles/sol", best_sol);
        cout << "MIDI file(s) created" << endl;
    }
    //todo summary
    std::cout << "\n\nSummary: " << pieceParams->toString() << std::endl;
    return 0;
}