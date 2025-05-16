//
// Created by Damien Sprockeels on 16/05/2025.
//

#include "../headers/TonalPieceParameters.hpp"

string TonalPieceParameters::toString() const {
    string message = "Tonal Piece Parameters:\n";
    message += "Number of chords: " + to_string(size) + "\n";
    message += "Number of sections: " + to_string(nSections) + "\n";
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
    message += "\n";
    return message;
}

