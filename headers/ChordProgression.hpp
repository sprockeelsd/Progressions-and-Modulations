//
// Created by Damien Sprockeels on 02/07/2024.
//

#ifndef CHORDGENERATOR_CHORDPROGRESSION_HPP
#define CHORDGENERATOR_CHORDPROGRESSION_HPP

#include "ChordGeneratorUtilities.hpp"
#include "MusicalParts.hpp"

/**
 * This class represents a chord progression of tonal music in a given tonality. It takes as argument a search space
 * in the form of a Home object. It posts constraints to ensure that the rules of tonal harmony inside a tonality are
 * respected.
 *
 * It contains a variable arrays for chord degrees in the tonality. Additionally, it also contains subsets of the global
 * variable arrays for states, qualities and root notes of the chords that are tied to this section. It also contains
 * auxiliary variable arrays for the bass notes, whether the chord is chromatic or has a seventh, and each degree
 * constituting the chords.
 *
 * It requires as argument a starting position in the global piece, a duration, a tonality, and the global variable
 * arrays. It can take optional parameters for the minimum and maximum number of chromatic and seventh chords in the
 * piece.
 */
class ChordProgression {
private:
    int start;                                   /// the starting position of the progression in the global piece
    int duration;                                /// the number of chords to be generated in this tonality
    Tonality* tonality;                          /// the tonality of the piece

    /// optional parameters
    int minChromaticChords;                      /// the min number of chromatic chords that are allowed in the progression(V/x, bII, 6te_a)
    int maxChromaticChords;                      /// the max number of chromatic chords that are allowed in the progression
    int minSeventhChords;                        /// the min number of seventh chords that are allowed in the progression
    int maxSeventhChords;                        /// the max number of seventh chords that are allowed in the progression

    /// Decision variable array
    IntVarArray chords;                          /// the chords of the progression expressed as degrees (I -> VII + Vda, °7, V/X,bII,6te_a)

    /// Subsets of global variable arrays
    IntVarArray states;                          /// the states of the chords (fundamental, first inversion, ...)
    IntVarArray qualities;                       /// the quality of the chords
    IntVarArray qualitiesWithoutSeventh;         /// the quality of the chords without the seventh
    IntVarArray rootNotes;                       /// the root notes
    IntVarArray hasSeventh;                      /// whether the chord has a seventh or not

    /// Auxiliary variable arrays
    IntVarArray bassDegrees;                     /// the bass notes corresponding to the chord degrees
    IntVarArray roots;                           /// the root notes corresponding to the chord degrees
    IntVarArray thirds;                          /// the third notes corresponding to the chord degrees
    IntVarArray fifths;                          /// the fifth notes corresponding to the chord degrees
    IntVarArray sevenths;                        /// the seventh notes corresponding to the chord degrees
    IntVarArray isChromatic;                     /// whether the chord is chromatic or not


public:
    /**
     * Constructor for ChordProgression objects. It initializes the object with the given parameters, links the subset arrays
     * to the main arrays in the TonalPiece class, and posts the constraints.
     * @param home the search space
     * @param start the starting position of the progression in the global piece
     * @param duration the duration of the progression
     * @param tonality the tonality of the progression
     * @param states the states of the chords
     * @param qualities the qualities of the chords
     * @param qualitiesWithoutSeventh the qualities of the chords without the seventh
     * @param rootNotes the root notes of the chords
     * @param hasSeventh whether the chord has a seventh or not
     * @param minPercentChromaticChords the minimum percentage of chromatic chords in the progression
     * @param maxPercentChromaticChords the maximum percentage of chromatic chords in the progression
     * @param minPercentSeventhChords the minimum percentage of seventh chords in the progression
     * @param maxPercentSeventhChords the maximum percentage of seventh chords in the progression
     * @return a ChordProgression object
     */
    ChordProgression(Home home, int start, int duration, Tonality *tonality, IntVarArray states, IntVarArray qualities,
                     IntVarArray qualitiesWithoutSeventh, IntVarArray rootNotes, IntVarArray hasSeventh,
                     double minPercentChromaticChords, double maxPercentChromaticChords, double minPercentSeventhChords,
                     double maxPercentSeventhChords);

    /**
     * Copy constructor
     * @param home the search space
     * @param s a ChordProgression object
     */
    ChordProgression(Home home, ChordProgression &s);


    int getStart() const { return start; }

    int getDuration() const { return duration; }

    Tonality *getTonality() const { return tonality; }

    IntVarArray getChords() { return chords; }

    IntVarArray getStates() { return states; }

    IntVarArray getQualities() { return qualities; }

    IntVarArray getQualitiesWithoutSeventh() { return qualitiesWithoutSeventh; }

    IntVarArray getRootNotes() { return rootNotes; }

    IntVarArray getRoots() { return roots; }

    IntVarArray getThirds() { return thirds; }

    IntVarArray getFifths() { return fifths; }

    IntVarArray getSevenths() { return sevenths; }

    IntVarArray getBassDegrees() { return bassDegrees; }

    IntVarArray getIsChromatic() { return isChromatic; }

    IntVarArray getHasSeventh() { return hasSeventh; }

    /**
     * Returns a string with each of the object's field values as integers. For debugging
     * @brief toString
     * @return a string representation of the object
     */
    string toString() const;

    /**
     * Returns the object's representation in a more readable way, with degrees and states.
     * @brief pretty
     * @return a string representation of the object
     */
    string pretty() const;

};


#endif //CHORDGENERATOR_CHORDPROGRESSION_HPP
