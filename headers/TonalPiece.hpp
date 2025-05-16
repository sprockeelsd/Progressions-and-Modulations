//
// Created by Damien Sprockeels on 30/09/2024.
//

#ifndef CHORDGENERATOR_TONALPIECE_HPP
#define CHORDGENERATOR_TONALPIECE_HPP

#include "ChordGeneratorUtilities.hpp"
#include "Modulation.hpp"
#include "TonalPieceParameters.hpp"

/**
 * This class represents a tonal piece. It can have multiple tonalities, with modulations between them. It extends
 * the Gecode::Space class to create the search space for the problem. This class does not directly post constraints,
 * but does so by calling other classes (ChordProgression and Modulation).
 *
 * It contains variable arrays for states, qualities and root notes of chords, allowing the different tonalities to
 * communicate with each other, for modulations for example.
 * It instantiates a ChordProgression object for each tonality, which will post the constraints for chord progressions
 * in a single tonality.
 * It also instantiates a Modulation object for each modulation, which will post the constraints for the modulation based
 * on its type.
 *
 * The following input is required to create the model: the size of the piece, the tonalities, the starting position and
 * ending position of each modulation, as well as their type.
 */
class TonalPiece : public Space {
private:
    /// input parameters
    TonalPieceParameters* parameters;

    /// General variable arrays for the piece
    IntVarArray                     states;                      /// the states of the chords (fundamental, first inversion, ...)
    IntVarArray                     qualities;                   /// the quality of the chords (major, minor, ...)
    IntVarArray                     rootNotes;                   /// the root notes corresponding to the chord degrees
    IntVarArray                     hasSeventh;                  /// whether the chord has a seventh or not

    /// Auxiliary variable array
    IntVarArray                     qualitiesWithoutSeventh;       /// the quality of the chords without the seventh

    /// Problem objects for sections and modulations
    vector<ChordProgression *>      progressions;                /// the chord progression objects for each tonality
    vector<Modulation *>            modulations;                 /// the modulation objects for each modulation

public:
    /**
     * Constructor for TonalPiece objects.
     * It initialises the variable arrays, and links the auxiliary array to the main ones. Assuming the parameters are
     * correct, it computes the starting position and duration of each tonality, and creates the ChordProgression and
     * Modulation objects. It also posts the branching. It is done in this order: First, branch on the chord degrees
     * for each tonality, then branch on states and qualities if necessary.
     */
    TonalPiece(TonalPieceParameters* params);

    /**
     * @brief Copy constructor
     * @param s a ChordProgression object pointer
     * Returns a TonalPiece object that is equivalent to s
     */
    TonalPiece(TonalPiece &s);

    TonalPieceParameters* getParameters() const { return parameters; };

    IntVarArray getStates() const { return states; };

    IntVarArray getQualities() const { return qualities; };

    IntVarArray getRootNotes() const { return rootNotes; };

    IntVarArray getHasSeventh() const { return hasSeventh; };

    IntVarArray getQualityWithoutSeventh() const { return qualitiesWithoutSeventh; };

    ChordProgression *getChordProgression(int pos) const { return progressions[pos]; };

    Modulation *getModulation(int pos) const { return modulations[pos]; };

    /**
     * Returns a string with each of the object's field values as integers.
     * @brief toString
     * @return a string representation of the object
     */
    string toString() const;

    /**
     * Returns a string representing the piece in a prettier format, more readable.
     * @brief pretty function
     * @return a string representation of the object
     */
    string pretty() const;

    /**
     * @brief copy function
     * @return a Space* object that is a copy of the current object
     */
    Space* copy() override;

};


#endif //CHORDGENERATOR_TONALPIECE_HPP
