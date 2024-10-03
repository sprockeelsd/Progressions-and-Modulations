//
// Created by Damien Sprockeels on 03/07/2024.
//

#ifndef CHORDGENERATOR_CONSTRAINTS_HPP
#define CHORDGENERATOR_CONSTRAINTS_HPP

#include "ChordGeneratorUtilities.hpp"

/***********************************************************************************************************************
 *                                        Linker functions                                                             *
 ***********************************************************************************************************************/

/**
 * Links chord qualities to the degrees
 * The quality of each chord is linked to its degree (V is major/7, I is major,...)
 * formula: majorDegreeQualities[chords[i] * nSupportedQualities + qualities[i]] = 1
 * @param home the problem space
 * @param chords the array of chord degrees
 * @param qualities the array of chord qualities
 */
void link_chords_to_qualities(const Home &home, int size, IntVarArray chords, IntVarArray qualities);

/**
 * Links chord states to the degrees
 * The state of each chord is linked to its degree (I can be in fund/1st inversion, VI can be in fund,...)
 * formula: majorDegreeStates[chords[i] * nSupportedStates + states[i]] = 1
 * @param home the problem space
 * @param chords the array of chord degrees
 * @param states the array of chord states
 */
void link_chords_to_states(const Home &home, int size, IntVarArray chords, IntVarArray states);

/**
 * The state of each chord is linked to its quality (7th chords can be in 3rd inversion, etc)
 * formula: qualitiesToStates[qualities[i] * nSupportedStates + states[i]] = 1
 * @param home the problem space
 * @param qualities the array of chord qualities
 * @param states the array of chord states
 */
void
link_states_to_qualities(const Home &home, int size, IntVarArray states, IntVarArray hasSeventh);

/**
 * Links the bass note to the corresponding degree and state.
 * @param home the problem space
 * @param size the number of chords in the tonality
 * @param startPosition the starting position of the tonality
 * @param chords the array of chord degrees
 * @param states the array of chord states
 * @param bassNotes the array of bass notes
 */
void link_bass_degrees_to_degrees_and_states(const Home &home, int size, IntVarArray chords, IntVarArray states,
                                             IntVarArray bassNotes);

/**
 * Link the chromatic chords array to the chords array, and constraints the number of chromatic chords to be equal to nChromaticChords
 * formula: isChromatic[i] == 1 <=> chords[i] >= FIVE_OF_TWO
 * formula: minChromaticChords <= sum(isChromatic) <= maxChromaticChords
 * @param home the problem space
 * @param size the number of chords
 * @param chords the array of chord degrees
 * @param isChromatic the array of chromatic chords
 * @param minChromaticChords the min number of chromatic chords we want
 * @param maxChromaticChords the max number of chromatic chords we want
 */
void chromatic_chords(const Home &home, int size, IntVarArray chords, IntVarArray isChromatic, int minChromaticChords,
                      int maxChromaticChords);

/**
 * Link the seventh chords and count them so that there are exactly nSeventhChords
 * formula: hasSeventh[i] == 1 <=> qualities[i] >= DOMINANT_SEVENTH_CHORD
 * formula: minSeventhChords <= sum(hasSeventh) <= maxSeventhChords
 * @param home the problem space
 * @param size the number of chords
 * @param hasSeventh the array of seventh chords
 * @param qualities the array of chord qualities
 * @param minSeventhChords the min number of seventh chords we want
 * @param maxSeventhChords the max number of seventh chords we want
 */
void seventh_chords(const Home &home, int size, IntVarArray hasSeventh, IntVarArray qualities, int minSeventhChords,
                    int maxSeventhChords);

/***********************************************************************************************************************
 *                                                   Constraints                                                       *
 ***********************************************************************************************************************/

/**
 * Enforces that chord progression are valid
 * chord[i] -> chord[i+1] is possible (matrix)
 * formula: tonalTransitions[chords[i] * nSupportedChords + chords[i + 1]] = 1
 * @param home the problem space
 * @param size the number of chords
 * @param chords the array of chord degrees
 */
void chord_transitions(const Home &home, int size, IntVarArray chords);

/**
 * Force the last chord to be diatonic and not the seventh chord
 * The chord progression cannot end on something other than a diatonic chord (also not seventh degree)
 * formula: chords[size - 1] < SEVENTH_DEGREE
 * @param home the problem space
 * @param size the number of chords
 * @param chords the array of chord degrees
 */
void last_chord_cst(const Home& home, int size, const IntVarArray& chords);

/**
 * Enforces that the fifth degree appogiatura (Vda) is followed by the fifth degree as a major or dominant seventh chord in fundamental state (V5/7+)
 * formula: chords[i] == I => states[i+1] == fund && qualities[i+1] == M/7
 * @param home the problem space
 * @param size the number of chords
 * @param chords the array of chord degrees
 * @param states the array of chord states
 * @param qualities the array of chord qualities
 */
void
fifth_degree_appogiatura(const Home &home, int size, IntVarArray chords, IntVarArray states, IntVarArray qualities);

/**
 * bII should be in first inversion todo maybe make this a preference?
 * @param home the problem space
 * @param size the number of chords
 * @param chords the array of chord degrees
 * @param states the array of chord states
 */
void flat_II_cst(const Home &home, int size, IntVarArray chords, IntVarArray states);

/**
 * If two successive chords are the same degree, they cannot have the same state or the same quality
 * formula: chords[i] = chords[i+1] => states[i] != states[i+1] || qualities[i] != qualities[i+1]
 * @param home the problem space
 * @param size the number of chords
 * @param chords the array of chord degrees
 * @param states the array of chord states
 * @param qualities the array of chord qualities
 */
void successive_chords_with_same_degree(const Home &home, int size, IntVarArray chords, IntVarArray states,
                                        IntVarArray qualities);

/**
 * Makes sure the state of the chords allows for tritone resolutions in the cases where it is necessary
 * cases handeld for now:
 *      - V65/-> I5 or V/IV5 (formula: chords[i] = V & states[i] = 1st) => chords[i+1] = I || V/IV & states[i+1] = fund)
 * @param home the problem space
 * @param size the number of chords
 * @param chords the array of chord degrees
 * @param states the array of chord states
 */
void tritone_resolutions(const Home &home, int size, IntVarArray chords, IntVarArray states);

/**
 * The fifth degree chord cannot be in second inversion if it is not a dominant seventh chord
 * formula: chords[i] == V && qualities[i] < DOMINANT_SEVENTH_CHORD => states[i] != SECOND_INVERSION
 * @param home the problem space
 * @param size the number of chords
 * @param chords the array of chord degrees
 * @param states the array of chord states
 * @param qualities the array of chord qualities
 */
void fifth_degree(const Home &home, int size, IntVarArray chords, IntVarArray states, IntVarArray qualities);

/***********************************************************************************************************************
 *                                            Optional Constraints (preferences)                                       *
 ***********************************************************************************************************************/

/**
 * Enforces different cadence types at a given position
 * @param home the problem space
 * @param position the position of the first chord of the cadence
 * @param type the type of cadence
 * @param chords the array of chord degrees
 * @param states the array of chord states
 * @param hasSeventh the array of seventh chords
 */
void cadence(const Home& home, int position, int type, IntVarArray chords, IntVarArray states, IntVarArray hasSeventh);

#endif //CHORDGENERATOR_CONSTRAINTS_HPP