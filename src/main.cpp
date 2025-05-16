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
    std::cout << params.toString() << std::endl;

    auto tonalPiece = new TonalPiece(&params);

    const auto sol = solve_harmoniser(tonalPiece);
    std::cout << "Best solution: \n" << sol->pretty() << std::endl;

    /// Link with diatony
    vector<int> phrasesStarts;
    vector<int> phrasesEnds;
    phrasesStarts.reserve(params.get_nProgressions());      phrasesEnds.reserve(params.get_nProgressions());
    phrasesStarts.push_back(0); // start of the first phrase
    for (int i = 0; i < params.get_nProgressions() - 1; i++) { // for each modulation
        switch (params.get_modulationType(i)) {
            case PERFECT_CADENCE_MODULATION: // the phrase ends with the last chord of the modulation
                {
                int last_chord_of_mod = params.get_modulationEnd(i);
                phrasesEnds.push_back(last_chord_of_mod);
                phrasesStarts.push_back(last_chord_of_mod + 1);
                break;
            }
            case PIVOT_CHORD_MODULATION: // the phrase ends before the first chord of the modulation. This ensures that the modulation is voiced smoothly, as all chords are in the second tonality
            case ALTERATION_MODULATION: // the phrase ends before the first chord of the modulation. because we abruptly change tonalities
            case SECONDARY_DOMINANT_MODULATION: // the phrase ends before the first chord of the modulation. This ensures proper voicing for the secondary dominant resolution.
                //todo test that these work properly
                {
                int first_chord_of_mod = params.get_modulationStart(i);
                phrasesEnds.push_back(first_chord_of_mod - 1);
                phrasesStarts.push_back(first_chord_of_mod);
                break;
            }

                break;

            default:
                throw std::invalid_argument("The modulation type is not recognized.");
        }
    }
    phrasesEnds.push_back(params.get_size()-1);

    std::cout << "Phrases starts: " << int_vector_to_string(phrasesStarts) << std::endl;
    std::cout << "Phrases ends: " << int_vector_to_string(phrasesEnds) << std::endl;

    return 0;
}