//
// Created by Damien Sprockeels on 16/05/2025.
//

#include "../headers/TonalPieceParameters.hpp"


TonalPieceParameters::TonalPieceParameters(int size, int nSections, const vector<Tonality*>& tonalities,
                     const vector<int>& modulationTypes,
                     const vector<int>& modulationStarts,
                     const vector<int>& modulationEnds) :
        size(size), nProgressions(nSections), tonalities(tonalities),
        modulationTypes(modulationTypes),
        modulationStarts(modulationStarts),
        modulationEnds(modulationEnds) {

    ///Compute tonality starts and durations
    progressionsStarts.reserve(nProgressions);    progressionsDurations.reserve(nProgressions);
    /// the first tonality starts at the beginning
    progressionsStarts.push_back(0);
    for(int i = 0; i < nProgressions - 1; i++){
        switch (modulationTypes[i]){
                /**
                 * The modulation lasts 2 chords, and the next tonality starts on the chord after the modulation
                 * example: C Major (I ... V I) (I ...) G Major
                 */
            case PERFECT_CADENCE_MODULATION:
                progressionsStarts        .push_back(modulationEnds[i] + 1);                       ///start of the next tonality
                progressionsDurations     .push_back(modulationEnds[i] - progressionsStarts[i] + 1); ///duration of the current tonality
                break;
                /**
                 * The modulation lasts at least 3 chords, and the next tonality starts on the first chord while the
                 * first tonality ends just before the perfect cadence, so there is an overlap of the tonalities
                 * example: C Major (I ... (VI) V I ...) G Major
                 */
            case PIVOT_CHORD_MODULATION:
                progressionsStarts        .push_back(modulationStarts[i]);
                progressionsDurations     .push_back(modulationEnds[i] -2 - progressionsStarts[i] + 1);
                break;
                /**
                 * The modulation lasts 3 chords, and the next tonality starts on the first chord while the first
                 * tonality ends just before the modulation. That is because the V chord might not be possible right
                 * after the alteration.
                 * example: C Major (I ... V I) (IV V ...) F Major
                 */
            case ALTERATION_MODULATION:
                progressionsStarts        .push_back(modulationStarts[i]);
                progressionsDurations     .push_back(modulationStarts[i] - progressionsStarts[i]);
                break;
                /**
                 * The modulation lasts 2 chords, and the next tonality starts on the first chord while the first
                 * tonality ends on the first chord. There is a 1 chord overlap, that is a secondary dominant in the
                 * first tonality and the V chord in the new tonality.
                 * example: C Major (I ... (V/V) I ...) G Major
                 */
            case SECONDARY_DOMINANT_MODULATION:
                progressionsStarts        .push_back(modulationStarts[i]);
                progressionsDurations     .push_back(modulationStarts[i] - progressionsStarts[i] +1);
                break;
            default:
                throw std::invalid_argument("The modulation type is not recognized.");
        }
    }
    ///the last section lasts until the end
    progressionsDurations.push_back(size - progressionsStarts[progressionsStarts.size()-1]);

    std::cout << "tonalitiesStarts: " << int_vector_to_string(progressionsStarts) << std::endl;
    std::cout << "tonalitiesDurations: " << int_vector_to_string(progressionsDurations) << std::endl;
}

string TonalPieceParameters::toString() const {
    string message = "Tonal Piece Parameters:\n";
    message += "Number of chords: " + to_string(size) + "\n";
    message += "Number of sections: " + to_string(nProgressions) + "\n";
    message += "Tonalities: ";
    for (const auto& t : tonalities) {
        message += t->get_name() + " ";
    }
    message += "\nModulation types: ";
    for (const auto& m : modulationTypes) {
        message += to_string(m) + " ";
    }
    message += "\nModulation starts: ";
    for (const auto& m : modulationStarts) {
        message += to_string(m) + " ";
    }
    message += "\nModulation ends: ";
    for (const auto& m : modulationEnds) {
        message += to_string(m) + " ";
    }
    message += "\nProgression starts: ";
    for (const auto& p : progressionsStarts) {
        message += to_string(p) + " ";
    }
    message += "\nProgression durations: ";
    for (const auto& p : progressionsDurations) {
        message += to_string(p) + " ";
    }
    message += "\n";
    return message;
}

