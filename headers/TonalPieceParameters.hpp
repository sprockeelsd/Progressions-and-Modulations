//
// Created by Damien Sprockeels on 16/05/2025.
//

#ifndef TONALPIECEPARAMETERS_HPP
#define TONALPIECEPARAMETERS_HPP

#include "ChordGeneratorUtilities.hpp"

/**
 * This class represents the parameters of a tonal piece, that is all the information related to the different progressions,
 * tonalities and modulations in the piece.
 */
class TonalPieceParameters {
protected:
    const int nChords;
    const int nProgressions;
    const vector<Tonality*> tonalities;
    const vector<int> modulationTypes;
    const vector<int> modulationStarts;
    const vector<int> modulationEnds;

    vector<int> progressionsStarts;
    vector<int> progressionsDurations;
    vector<int> phraseStarts;
    vector<int> phraseEnds;

public:
    /**
     * Constructor
     * @param nChords the total number of chords in the piece
     * @param nSections the total number of sections in the piece (i.e., the number of tonalities)
     * @param tonalities a vector of pointers to the tonalities of the piece
     * @param modulationTypes a vector of integers representing the type of modulation for each section
     * @param modulationStarts a vector of integers representing the start of each modulation
     * @param modulationEnds a vector of integers representing the end of each modulation
     */
    TonalPieceParameters(int nChords, int nSections, const vector<Tonality*>& tonalities,
        const vector<int>& modulationTypes, const vector<int>& modulationStarts, const vector<int>& modulationEnds);

    /**                        getters                        **/
    int         get_size() const                                { return nChords; }

    int         get_nProgressions() const                       { return nProgressions; }

    Tonality*   get_tonality(const int index) const             { return tonalities[index]; }

    int         get_modulationType(const int index) const       { return modulationTypes[index]; }

    int         get_modulationStart(const int index) const      { return modulationStarts[index]; }

    int         get_modulationEnd(const int index) const        { return modulationEnds[index]; }

    int         get_progressionStart(const int index) const     { return progressionsStarts[index]; }

    int         get_progressionDuration(const int index) const  { return progressionsDurations[index]; }

    int         get_phraseStart(const int index) const          { return phraseStarts[index]; }

    int         get_phraseEnd(const int index) const            { return phraseEnds[index]; }


    /**
     * ToString method
     * Prints the total number of chords in the piece, the tonality of each section, the modulations' type, start and end
     * @return a string representation of the object
     */
    string toString() const;
};

#endif //TONALPIECEPARAMETERS_HPP