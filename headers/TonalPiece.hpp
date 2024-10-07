//
// Created by Damien Sprockeels on 30/09/2024.
//

#ifndef CHORDGENERATOR_TONALPIECE_HPP
#define CHORDGENERATOR_TONALPIECE_HPP

#include "ChordGeneratorUtilities.hpp"
#include "ChordProgression.hpp"

/**
 * This class represents a tonal piece. It can have multiple tonalities, with modulations between them. It extends
 * the Gecode::Space class to create the search space for the problem. This class does not directly post constraints,
 * but does so by calling other classes (ChordProgression and Modulation).
 *
 * It contains variable arrays for states, qualities and root notes of chords, allowing the different tonalities to
 * communicate with each other, for modulations for example.
 * It instantiates a ChordProgression object for each tonality, which will post the constraints for chord progressions
 * in a single tonality.
 * Modulations objects will be added in the future to allow for modulations between tonalities.
 */
class TonalPiece : public Space {
private:
    int                         size;                /// the total number of chords to be generated

    vector<Tonality*>           tonalities;          /// the tonalities of the piece
    vector<int>                 tonalitiesStarts;    /// the starting position of each tonality
    vector<int>                 tonalitiesDurations; /// the duration of each tonality
    vector<int>                 modulationTypes;     /// the type of modulations that occurs between tonalities
    vector<int>                 modulationStarts;    /// the starting position of each modulation todo maybe not necessary

    /// These are general, not tonality specific.
    IntVarArray                 states;              /// the states of the chords (fundamental, first inversion, ...)
    IntVarArray                 qualities;           /// the quality of the chords (major, minor, ...)
    IntVarArray                 rootNotes;           /// the root notes corresponding to the chord degrees

    vector<ChordProgression *>  progressions;        /// the chord progression objects for each tonality

public:
    /**
     * Constructor for TonalPiece objects. It initializes the object with the given parameters, as well as the other
     * objects that post constraints. It also posts the branching for the global arrays. That branching is posted after
     * the ChordProgression objects have posted theirs.
     * @param size the total size of the piece in terms of number of chords
     * @param tonalities a vector of Tonality objects for each tonality of the piece
     * @param tonalitiesStarts a vector of integers representing the starting position of each tonality. They can overlap (modulations)
     * @param tonalitiesDurations a vector of integers representing the duration of each tonality
     * @param modulationTypes a vector of integers representing the type of modulation between the tonalities
     * @param modulationStarts a vector of integers representing the starting position of each modulation
     * @return a TonalPiece object
     */
    TonalPiece(int size, const vector<Tonality *>& tonalities, vector<int> tonalitiesStarts,
               vector<int> tonalitiesDurations, vector<int> modulationTypes,
               vector<int> modulationStarts);

    /**
     * @brief Copy constructor
     * @param s a ChordProgression object
     */
    TonalPiece(TonalPiece &s);

    /**
     * Returns a string with each of the object's field values as integers. For debugging
     * @brief toString
     * @return a string representation of the object
     */
    string toString() const;

    /**
     * @brief copy function
     * @return a Space* object that is a copy of the current object
     */
    Space* copy() override;

};


#endif //CHORDGENERATOR_TONALPIECE_HPP
