//
// Created by Damien Sprockeels on 16/05/2025.
//

#ifndef TONALPIECEPARAMETERS_HPP
#define TONALPIECEPARAMETERS_HPP

#include "ChordGeneratorUtilities.hpp"

class TonalPieceParameters {
protected:
    const int size;                       // The number of chords in the piece
    const int nProgressions;                 // The number of sections in the piece
    const vector<Tonality*> tonalities;   // The tonalities of the piece
    const vector<int> modulationTypes;
    const vector<int> modulationStarts;   // The starting position of each modulation
    const vector<int> modulationEnds;     // The ending position of each modulation

public:
    TonalPieceParameters(int size, int nSections, const vector<Tonality*>& tonalities,
                         const vector<int>& modulationTypes,
                         const vector<int>& modulationStarts,
                         const vector<int>& modulationEnds) :
            size(size), nProgressions(nSections), tonalities(tonalities),
            modulationTypes(modulationTypes),
            modulationStarts(modulationStarts),
            modulationEnds(modulationEnds) {}

    /**                        getters                        **/
    int get_size() const { return size; }

    int get_nProgressions() const { return nProgressions; }

    Tonality* get_tonality(int index) const { return tonalities[index]; }

    int get_modulationType(int index) const { return modulationTypes[index]; }

    int get_modulationStart(int index) const { return modulationStarts[index]; }

    int get_modulationEnd(int index) const { return modulationEnds[index]; }


    /**
     * ToString method
     * Prints the total number of chords in the piece, the tonality of each section, the modulations' type, start and end
     * @return a string representation of the object
     */
    string toString() const;


};



#endif //TONALPIECEPARAMETERS_HPP
