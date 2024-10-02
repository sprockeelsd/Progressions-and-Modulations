//
// Created by Damien Sprockeels on 30/09/2024.
//

#ifndef CHORDGENERATOR_TONALPIECE_HPP
#define CHORDGENERATOR_TONALPIECE_HPP

#include "ChordGeneratorUtilities.hpp"
#include "ChordProgression.hpp"

/**
 * This class represents a tonal piece. It can have multiple tonalities, with modulations between them.
 */
class TonalPiece : public Space {
private:
    int size;                        /// the total number of chords to be generated

    vector<Tonality*> tonalities;    /// the tonalities of the piece
    vector<int> tonalitiesStarts;    /// the starting position of each tonality
    vector<int> tonalitiesDurations; /// the duration of each tonality
    vector<int> modulationTypes;     /// the type of modulations that occurs between tonalities
    vector<int> modulationStarts;    /// the starting position of each modulation todo maybe not necessary

    /// These are general, not tonality specific. This is what the modulation constraints will influence
    IntVarArray states;                      /// the states of the chords (fundamental, first inversion, second inversion, third inversion)
    IntVarArray qualities;                   /// the quality of the chords todo link it with borrowed chords: if false, then default, else major/dominant 7
    IntVarArray bassDegrees;                 /// the degree of the note at the bass. It is linked to states but is useful to enforce preferences on the bass
    IntVarArray rootNotes;                   /// the root notes corresponding to the chord degrees. It is useful to enforce modulations

    vector<ChordProgression *> progressions; /// the chord progression objects for each tonality

    //vector<IntVarArray> isChromatic;         /// whether the chord is chromatic or not
    //vector<IntVarArray> hasSeventh;          /// whether the chord has a seventh or not

public:
    TonalPiece(int size, const vector<Tonality *>& tonalities, vector<int> tonalitiesStarts,
               vector<int> tonalitiesDurations, vector<int> modulationTypes,
               vector<int> modulationStarts);

    /**
     * Returns a string with each of the object's field values as integers. For debugging
     * @brief toString
     * @return a string representation of the object
     */
    string toString() const;

    /**
     * @brief Copy constructor
     * @param s a ChordProgression object
     */
    TonalPiece(TonalPiece &s);

    /**
     * @brief copy function
     * @return a Space* object that is a copy of the current object
     */
    Space* copy() override;

};


#endif //CHORDGENERATOR_TONALPIECE_HPP
