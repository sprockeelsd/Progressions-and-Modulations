//
// Created by Damien Sprockeels on 02/07/2024.
//

#include "../headers/ChordGeneratorUtilities.hpp"
#include "../Diatony/c++/headers/aux/MidiFileGeneration.hpp"

#include "../headers/HarmoniserSolver.hpp"

// todo ajouter les 64 de passage (cst en plus du coup)
int main(int argc, char **argv) {
    string four_voice = argv[1]; /// true if we want to generate the 4voice chords, false if we just want chords and state

    int size = 20;
    Tonality* Cminor = new MinorTonality(C);    Tonality* Ebmajor = new MajorTonality(E_FLAT);
    vector<Tonality*> tonalities = {Cminor, Ebmajor};
    vector<int> modulationTypes = {SECONDARY_DOMINANT_MODULATION};
    vector<int> modulationStarts = {12};
    vector<int> modulationEnds = {13};

    auto params = TonalPieceParameters(size, tonalities.size(), tonalities,
                                       modulationTypes, modulationStarts, modulationEnds);
    std::cout << params.toString() << std::endl;

    auto tonalPiece = new TonalPiece(&params);

    const auto sol = solve_harmoniser(tonalPiece);
    std::cout << "Best solution: \n" << sol->pretty() << std::endl;

    return 0;

    /// link with Diatony
    vector<TonalProgressionParameters*> sectionParams;
    sectionParams.reserve(tonalities.size());
    vector<ModulationParameters*> modulationParams;
    modulationParams.reserve(tonalities.size() - 1);

    // create the section parameters for diatony
    for (int i = 0; i < tonalities.size(); i++) {
        vector<int> chord_degrees = intVarArray_to_int_vector(sol->getChordProgression(i)->getChords());
        vector<int> chord_qualities = intVarArray_to_int_vector(sol->getChordProgression(i)->getQualities());
        vector<int> chord_states = intVarArray_to_int_vector(sol->getChordProgression(i)->getStates());

        auto sec_params = new TonalProgressionParameters(i, sol->getChordProgression(i)->getDuration(),
                                                         sol->getChordProgression(i)->getStart(),
                                                         sol->getChordProgression(i)->getStart() + sol->getChordProgression(i)->getDuration() - 1,
                                                         tonalities[i],
                                                         chord_degrees,
                                                         chord_qualities,
                                                         chord_states);
        std::cout << "Section " << i << " parameters: " << sec_params->to_string() << std::endl;
        sectionParams.push_back(sec_params);
    }

    // create the modulation parameters
    for (int i = 0; i < modulationTypes.size(); i++) {
        auto mod_params = new ModulationParameters(modulationTypes[i], modulationStarts[i], modulationEnds[i],
                                                    sectionParams[i], sectionParams[i+1]);

        std::cout << mod_params->toString() << std::endl;
        modulationParams.push_back(mod_params);
    }

    auto pieceParams = new FourVoiceTextureParameters(size, tonalities.size(), sectionParams, modulationParams);
    std::cout << pieceParams->toString() << std::endl;

    auto pb = new FourVoiceTexture(pieceParams);
    std::cout << "Problem created" << std::endl;
    std::cout << pb->to_string() << std::endl;

    Options opts;
    opts.threads = 1;
    opts.stop = Stop::time(60000); // stop after 120 seconds
    opts.cutoff = Cutoff::merge(
            Cutoff::linear(2*pieceParams->get_totalNumberOfChords()),
            Cutoff::geometric((4*pieceParams->get_totalNumberOfChords())^2, 2));
    opts.nogoods_limit = pieceParams->get_totalNumberOfChords() * 4 * 4;


    RBS<FourVoiceTexture, BAB> solver(pb, opts);
    delete pb;

    int n_sols = 0;
    FourVoiceTexture* lastSol = nullptr;
    auto start = std::chrono::high_resolution_clock::now();     /// start time
    while (FourVoiceTexture* sol_fvt = solver.next()) {
        n_sols += 1;
        lastSol = dynamic_cast<FourVoiceTexture *>(sol_fvt->copy());
        std::cout << sol_fvt->to_string() << std::endl;
        std::cout << statistics_to_string(solver.statistics()) << std::endl;
        //todo improve branching and search (see notes)
        //if (n_sols >= 1) break;
    }
    auto currTime = std::chrono::high_resolution_clock::now();     /// current time
    std::chrono::duration<double> duration = currTime - start;

    std::cout << "search over" << std::endl;
    if(solver.stopped()){
        std::cout << "Best solution not found within the time limit." << std::endl;
    }
    else if(n_sols == 0){
        std::cout << "No solutions" << std::endl;
    }
    else{
        std::cout << "Best solution found." << std::endl;
    }
    std::cout << "time taken: " << duration.count() << " seconds and " << n_sols << " solutions found.\n" << std::endl;

    if(lastSol != nullptr){
        writeSolToMIDIFile(lastSol->getParameters()->get_totalNumberOfChords(), "out/MidiFiles/sol", lastSol);
        cout << "MIDI file(s) created" << endl;
    }

    std::cout << "Summary: " << pieceParams->toString() << std::endl;

    return 0;
}