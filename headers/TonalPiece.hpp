//
// Created by Damien Sprockeels on 30/09/2024.
//

#ifndef CHORDGENERATOR_TONALPIECE_HPP
#define CHORDGENERATOR_TONALPIECE_HPP

#include "ChordGeneratorUtilities.hpp"
#include "Modulation.hpp"

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
 * ending position of each modulation, as well a their type.
 */
class TonalPiece : public Space {
private:
    /// input parameters
    int                             size;                        /// the total number of chords to be generated
    vector<Tonality*>               tonalities;                  /// the tonalities of each section in the piece
    vector<int>                     modulationTypes;             /// the type of modulations that occurs between tonalities
    vector<int>                     modulationStarts;            /// the starting position of each modulation
    vector<int>                     modulationEnds;              /// the ending position of each modulation

    /// computed based on input parameters
    vector<int>                     tonalitiesStarts;            /// the starting position of each tonality
    vector<int>                     tonalitiesDurations;         /// the duration of each tonality


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
     * @param size the total number of chords in the piece
     * @param tonalities a vector of Tonality objects for each section of the piece
     * @param modulationTypes a vector of integers representing the type of modulation between the tonalities
     * @param modulationStarts a vector of integers representing the starting position of each modulation
     * @param modulationEnds a vector of integers representing the ending position of each modulation
     */
    TonalPiece(int size, const vector<Tonality *> &tonalities, vector<int> modulationTypes,
               vector<int> modulationStarts, vector<int> modulationEnds);

    /**
     * @brief Copy constructor
     * @param s a ChordProgression object pointer
     * Returns a TonalPiece object that is equivalent to s
     */
    TonalPiece(TonalPiece &s);


    int getSize() const { return size; };

    Tonality *getTonality(int pos) const { return tonalities[pos]; };

    int getTonalityStart(int pos) const { return tonalitiesStarts[pos]; };

    int getTonalityDuration(int pos) const { return tonalitiesDurations[pos]; };

    int getModulationType(int pos) const { return modulationTypes[pos]; };

    int getModulationStart(int pos) const { return modulationStarts[pos]; };

    int getModulationEnd(int pos) const { return modulationEnds[pos]; };

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
