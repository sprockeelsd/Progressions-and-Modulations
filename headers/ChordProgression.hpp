//
// Created by Damien Sprockeels on 02/07/2024.
//

#ifndef CHORDGENERATOR_CHORDPROGRESSION_HPP
#define CHORDGENERATOR_CHORDPROGRESSION_HPP

#include "ChordGeneratorUtilities.hpp"
#include "MusicalParts.hpp"

/**
 * This class represents a chord progression of tonal music in a given tonality. It takes as argument a search space
 * in the form of a Home object. It posts constraints to ensure that the rules of tonal harmony are respected.
 *
 * It contains a series of parameters, as well as variable arrays. The states, qualities and rootNotes are slices of the
 * TonalPiece object that creates this object, while the chords and bassDegrees are specific to this object.
 * It also has auxiliary variables arrays, isChromatic and hasSeventh, that are used for preferences.
 */
class ChordProgression {
private:
    int start;                       /// the starting position of the progression in the global piece
    int duration;                    /// the number of chords to be generated in this tonality
    int minChromaticChords;          /// the min number of chromatic chords that are allowed in the progression(V/x, bII, 6te_a)
    int maxChromaticChords;          /// the max number of chromatic chords that are allowed in the progression
    int minSeventhChords;            /// the min number of seventh chords that are allowed in the progression todo maybe dissociate V and V/X chords from others
    int maxSeventhChords;            /// the max number of seventh chords that are allowed in the progression

    Tonality* tonality;              /// the tonality of the piece

    IntVarArray chords;              /// the chords of the progression expressed as degrees (I -> VII + Vda, °7, V/X,bII,6te_a)
    IntVarArray states;              /// the states of the chords (fundamental, first inversion, ...)
    IntVarArray qualities;           /// the quality of the chords
    IntVarArray qualitiesWithoutSeventh; /// the quality of the chords without the seventh
    IntVarArray bassDegrees;         /// the bass notes corresponding to the chord degrees
    IntVarArray rootNotes;           /// the root notes

    IntVarArray isChromatic;         /// whether the chord is chromatic or not
    IntVarArray hasSeventh;          /// whether the chord has a seventh or not

public:
    /**
     * Constructor for ChordProgression objects. It initializes the object with the given parameters, and posts the
     * constraints.
     * @param home the search space
     * @param tonality the tonality of the progression
     * @param start the starting position of the progression in the global piece
     * @param duration the duration of the progression
     * @param states the states of the chords
     * @param qualities the qualities of the chords
     * @param rootNotes the root notes of the chords
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

    /**
     * getters
     * @return the attributes of the object
     */

    int getStart() const { return start; }

    int getDuration() const { return duration; }

    Tonality *getTonality() const { return tonality; }

    IntVarArray getChords() { return chords; }

    IntVarArray getStates() { return states; }

    IntVarArray getQualities() { return qualities; }

    IntVarArray getQualitiesWithoutSeventh() { return qualitiesWithoutSeventh; }

    IntVarArray getRootNotes() { return rootNotes; }

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
