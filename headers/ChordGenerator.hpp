//
// Created by Damien Sprockeels on 02/07/2024.
//

#ifndef CHORDGENERATOR_CHORDGENERATOR_HPP
#define CHORDGENERATOR_CHORDGENERATOR_HPP

#include "ChordGeneratorUtilities.hpp"
#include "MusicalParts.hpp"

/**
 * Chord generator constructor
 * @param s the number of chords to be generated
 * @param tonality the tonality of the piece
 * @param maxPercentChromaticChords lower bound on the percentage of chromatic chords in the progression
 * @param maxPercentChromaticChords upper bound on the percentage of chromatic chords in the progression
 * @param minPercentSeventhChords lower bound on the percentage of seventh chords in the progression
 * @param maxPercentSeventhChords upper bound on the percentage of seventh chords in the progression
 */
class ChordGenerator : public Space{
private:
    int size;                        /// the number of chords to be generated
    int minChromaticChords;          /// the min number of chromatic chords that are allowed in the progression(V/x, bII, 6te_a)
    int maxChromaticChords;          /// the max number of chromatic chords that are allowed in the progression(V/x, bII, 6te_a)
    int minSeventhChords;            /// the min number of seventh chords that are allowed in the progression todo maybe dissociate V and V/X chords from others
    int maxSeventhChords;            /// the max number of seventh chords that are allowed in the progression todo maybe dissociate V and V/X chords from others
    vector<Tonality*> tonalities;    /// the tonality of the piece
    vector<int> tonalities_starts;   /// the starting degree of each tonality

    IntVarArray chords;              /// the chords of the progression expressed as degrees (I -> VII)
    IntVarArray states;              /// the states of the chords (fundamental, first inversion, second inversion, third inversion)
    IntVarArray qualities;           /// the quality of the chords todo link it with borrowed chords: if false, then default, else major/dominant 7

    IntVarArray isChromatic;         /// whether the chord is chromatic or not
    IntVarArray hasSeventh;          /// whether the chord has a seventh or not

public:
    /**
     * @brief ChordGenerator
     * @param s the number of chords to be generated
     * @param tonalities the tonality of the piece
     */
    ChordGenerator(int s, vector<Tonality *> tonalities, vector<int> tonalities_starts,
                   double minPercentChromaticChords, double maxPercentChromaticChords,
                   double minPercentSeventhChords, double maxPercentSeventhChords);

    /**
     * @brief ChordGenerator
     * @param s a ChordGenerator object
     */
    ChordGenerator(ChordGenerator &s);

    /**
     * @brief copy
     * @return a Space* object that is a copy of the current object
     */
    virtual Space* copy();

    /**
     * @brief getSize
     * @return the size of the chord progression
     */
    int getSize() const { return size; }
    /**
     * @brief getChords
     * @return the chords of the progression
     */
    IntVarArray getChords() const { return chords; }
    /**
     * @brief getStates
     * @return the states of the chords
     */
    IntVarArray getStates() const { return states; }

    /**
     * @brief getQualities
     * @return the qualities of the chords
     */
    IntVarArray getQualities() const { return qualities; }

    /**
     * Returns a string with each of the object's field values as integers. For debugging
     * @brief toString
     * @return a string representation of the object
     */
    string toString() const;

    /**
     * Returns the object's representation in a more readable way, with degrees, state and whether the chord is borrowed
     * as strings instead of integers.
     * @brief pretty
     * @return a string representation of the object
     */
    string pretty() const;

};


#endif //CHORDGENERATOR_CHORDGENERATOR_HPP
