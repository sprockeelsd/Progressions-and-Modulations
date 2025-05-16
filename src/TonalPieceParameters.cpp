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
    ///Compute phrases starts and ends

    phraseStarts.reserve(nProgressions);      phraseEnds.reserve(nProgressions);
    /// the first tonality and phrase start at the beginning
    progressionsStarts.push_back(0);        phraseStarts.push_back(0);
    for(int i = 0; i < nProgressions - 1; i++){
        switch (modulationTypes[i]){
                /**
                 * The modulation lasts 2 chords, and the next tonality starts on the chord after the modulation
                 * example: C Major (I ... V I) (I ...) G Major
                 */
            case PERFECT_CADENCE_MODULATION: {
                progressionsStarts        .push_back(modulationEnds[i] + 1);                       ///start of the next tonality
                progressionsDurations     .push_back(modulationEnds[i] - progressionsStarts[i] + 1); ///duration of the current tonality
                // the phrase ends with the last chord of the modulation
                int last_chord_of_mod = modulationEnds[i];
                phraseEnds.push_back(last_chord_of_mod);
                phraseStarts.push_back(last_chord_of_mod + 1);
                break;
            }
                /**
                 * The modulation lasts at least 3 chords, and the next tonality starts on the first chord while the
                 * first tonality ends just before the perfect cadence, so there is an overlap of the tonalities
                 * example: C Major (I ... (VI) V I ...) G Major
                 */
            case PIVOT_CHORD_MODULATION: {
                progressionsStarts        .push_back(modulationStarts[i]);
                progressionsDurations     .push_back(modulationEnds[i] -2 - progressionsStarts[i] + 1);
                // the phrase ends before the first chord of the modulation. This ensures that the modulation is voiced smoothly, as all chords are in the second tonality
                int first_chord_of_mod = modulationStarts[i];
                phraseEnds.push_back(first_chord_of_mod - 1);
                phraseStarts.push_back(first_chord_of_mod);
                break;
            }
                /**
                 * The modulation lasts 3 chords, and the next tonality starts on the first chord while the first
                 * tonality ends just before the modulation. That is because the V chord might not be possible right
                 * after the alteration.
                 * example: C Major (I ... V I) (IV V ...) F Major
                 */
            case ALTERATION_MODULATION: {
                progressionsStarts        .push_back(modulationStarts[i]);
                progressionsDurations     .push_back(modulationStarts[i] - progressionsStarts[i]);
                // the phrase ends before the first chord of the modulation. because we abruptly change tonalities
                int first_chord_of_mod = modulationStarts[i];
                phraseEnds.push_back(first_chord_of_mod - 1);
                phraseStarts.push_back(first_chord_of_mod);
                break;
            }
                /**
                 * The modulation lasts 2 chords, and the next tonality starts on the first chord while the first
                 * tonality ends on the first chord. There is a 1 chord overlap, that is a secondary dominant in the
                 * first tonality and the V chord in the new tonality.
                 * example: C Major (I ... (V/V) I ...) G Major
                 */
            case SECONDARY_DOMINANT_MODULATION: {
                progressionsStarts        .push_back(modulationStarts[i]);
                progressionsDurations     .push_back(modulationStarts[i] - progressionsStarts[i] +1);
                // the phrase ends before the first chord of the modulation. This ensures proper voicing for the secondary dominant resolution.
                int first_chord_of_mod = modulationStarts[i];
                phraseEnds.push_back(first_chord_of_mod - 1);
                phraseStarts.push_back(first_chord_of_mod);
                break;
            }
            default:
                throw std::invalid_argument("The modulation type is not recognized.");
        }
    }
    ///the last section lasts until the end
    progressionsDurations.push_back(size - progressionsStarts[progressionsStarts.size()-1]);
    phraseEnds.push_back(size-1);
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
    message += "\nPhrase starts: ";
    for (const auto& p : phraseStarts) {
        message += to_string(p) + " ";
    }
    message += "\nPhrase ends: ";
    for (const auto& p : phraseEnds) {
        message += to_string(p) + " ";
    }
    message += "\n";
    return message;
}

