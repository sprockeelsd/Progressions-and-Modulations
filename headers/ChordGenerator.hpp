//
// Created by Damien Sprockeels on 02/07/2024.
//

#ifndef CHORDGENERATOR_CHORDGENERATOR_HPP
#define CHORDGENERATOR_CHORDGENERATOR_HPP

#include "ChordGeneratorUtilities.hpp"
#include "Constraints.hpp"

/**
 * @brief The ChordGenerator class
 * This class models the harmonic problem of writing a tonal chord progression. It takes as argument a size and a tonality,
 * and generates a chord progression in that tonality.
 */
class ChordGenerator : public Space{
private:
    int size;                       /// the number of chords to be generated
    Tonality* tonality;             /// the tonality of the piece
    IntVarArray chords;             /// the chords of the progression expressed as degrees (I -> VII)
    IntVarArray states;             /// the states of the chords (fundamental, first inversion, second inversion, third inversion)
    IntVarArray qualities;          /// todo link it with borrowed chords: if false, then default, else major/dominant 7
    IntVarArray borrowedChords;     /// wether the chord is borrowed. If borrowed, the chord is either major or dominant 7th and resolved in the next chord

public:
    /**
     * @brief ChordGenerator
     * @param size the number of chords to be generated
     * @param tonality the tonality of the piece
     */
    ChordGenerator(int size, Tonality* tonality);

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
     * @brief getTonality
     * @return the tonality of the chord progression
     */
    Tonality* getTonality() const { return tonality; }
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
