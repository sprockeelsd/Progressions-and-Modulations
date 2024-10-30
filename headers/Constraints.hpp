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
 * Links the chord qualities to the degrees
 * The quality of each chord is linked to its degree (V is major/7, I is major,...)
 * formula: majorDegreeQualities[chords[i] * nSupportedQualities + qualities[i]] = 1
 * todo also make it work for minor tonalities
 * @param home the problem space
 * @param size the number of chords in the progression
 * @param qualities the array of chord qualities
 * @param chords the array of chord degrees
 */
void link_chords_to_qualities(const Home &home, int size, IntVarArray qualities, IntVarArray chords);

/**
 * Links chord states to the degrees
 * The state of each chord is linked to its degree (I can be in fund/1st inversion, VI can be in fund,...)
 * formula: majorDegreeStates[chords[i] * nSupportedStates + states[i]] = 1
 * todo also make it work for minor tonalities
 * @param home the problem space
 * @param size the number of chords in the progression
 * @param states the array of chord states
 * @param chords the array of chord degrees
 */
void link_chords_to_states(const Home &home, int size, IntVarArray states, IntVarArray chords);

/**
 * Links chord states to qualities
 * The state of each chord is linked to its quality (3 note chords cannot be in 3rd inversion)
 * formula: hasSeventh[i] == 0 => states[i] < THIRD_INVERSION
 * @param home the problem space
 * @param size the number of chords in the progression
 * @param states the array of chord states
 * @param hasSeventh the array of whether chords have a seventh
 */
void link_states_to_qualities(const Home &home, int size, IntVarArray states, IntVarArray hasSeventh);

/**
 * Links the root note to the corresponding degree. It makes the connection between tonalities by referring directly to notes
 * formula: rootNotes[i] = tonality->get_degree_note(chords[i])
 * @param home the problem space
 * @param size the number of chords in the progression
 * @param tonality the tonality of the progression
 * @param rootNotes the slice of the rootNote array corresponding to this tonality
 * @param chords the array of chord degrees
 */
void link_root_notes_to_degrees(const Home &home, int size, Tonality *tonality, IntVarArray rootNotes, IntVarArray chords);

/**
 * Links the bass degree to the corresponding chord degree and state.
 * formula: bassBasedOnDegreeAndState[chords[i] * nSupportedStates + states[i]] = bassDegrees[i]
 * @param home the problem space
 * @param size the number of chords in the progression
 * @param states the array of chord states
 * @param bassDegrees the array of bass degrees
 * @param chords the array of chord degrees
 */
void link_bass_degrees_to_degrees_and_states(const Home &home, int size, IntVarArray states, IntVarArray chords,
                                             IntVarArray bassDegrees);

/**
 * Link the chromatic chords array to the chords array, and constraints the number of chromatic chords to be in the range
 * [minChromaticChords, maxChromaticChords]
 * formula: isChromatic[i] == 1 <=> chords[i] >= FIVE_OF_TWO
 * formula: minChromaticChords <= sum(isChromatic) <= maxChromaticChords
 * @param home the problem space
 * @param size the number of chords in the progression
 * @param chords the array of chord degrees
 * @param isChromatic the array of chromatic chords
 * @param minChromaticChords the min number of chromatic chords we want
 * @param maxChromaticChords the max number of chromatic chords we want
 */
void chromatic_chords(const Home &home, int size, IntVarArray chords, IntVarArray isChromatic, int minChromaticChords,
                      int maxChromaticChords);

/**
 * Link the seventh chords and constraints the number of seventh chords to be in the range [minSeventhChords, maxSeventhChords]
 * formula: hasSeventh[i] == 1 <=> qualities[i] >= DOMINANT_SEVENTH_CHORD
 * formula: minSeventhChords <= sum(hasSeventh) <= maxSeventhChords
 * @param home the problem space
 * @param size the number of chords in the progression
 * @param qualities the array of chord qualities
 * @param hasSeventh the array of seventh chords
 * @param minSeventhChords the min number of seventh chords we want
 * @param maxSeventhChords the max number of seventh chords we want
 */
void seventh_chords(const Home &home, int size, IntVarArray qualities, IntVarArray hasSeventh, int minSeventhChords,
                    int maxSeventhChords);

/**
 * Links the qualities array to the quality_without_seventh array. This is useful when you need to know the general
 * quality of the chord but don't need to know if it has a seventh
 * @param home the problem space
 * @param size the number of chords in the progression
 * @param qualities the array of chord qualities
 * @param qualityWithoutSeventh the array of chord qualities without the seventh
 */
void link_qualities_to_3note_version(const Home &home, int size, IntVarArray qualities, IntVarArray qualityWithoutSeventh);

/***********************************************************************************************************************
 *                                                   Constraints                                                       *
 ***********************************************************************************************************************/

/**
 * Enforces that the chord progression is valid. That is, each transition between successive chords is possible
 * chord[i] -> chord[i+1] is possible (matrix)
 * formula: tonalTransitions[chords[i] * nSupportedChords + chords[i + 1]] = 1
 * @param home the problem space
 * @param size the number of chords in this tonality
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
 * Enforces that the fifth degree appogiatura (Vda) is followed by the fifth degree as a major or dominant seventh chord
 * in fundamental state (V5/7+)
 * formula: chords[i] == I => states[i+1] == fund && qualities[i+1] == M/7
 * @param home the problem space
 * @param size the number of chords
 * @param states the array of chord states
 * @param qualities the array of chord qualities
 * @param chords the array of chord degrees
 */
void fifth_degree_appogiatura(const Home &home, int size, IntVarArray states, IntVarArray qualities, IntVarArray chords);

/**
 * bII should be in first inversion todo maybe make this a preference?
 * @param home the problem space
 * @param size the number of chords
 * @param states the array of chord states
 * @param chords the array of chord degrees
 */
void flat_II_cst(const Home &home, int size, IntVarArray states, IntVarArray chords);

/**
 * If two successive chords are the same degree, they cannot have the same state or the same quality
 * formula: chords[i] = chords[i+1] => states[i] != states[i+1] || qualities[i] != qualities[i+1]
 * The same degree cannot happen more than twice successively
 * formula: chords[i] == chords[i+1] => chords[i+2] != chords[i]
 * @param home the problem space
 * @param size the number of chords
 * @param states the array of chord states
 * @param qualities the array of chord qualities
 * @param chords the array of chord degrees
 */
void successive_chords_with_same_degree(const Home &home, int size, IntVarArray states, IntVarArray qualities,
                                        IntVarArray chords);

/**
 * Makes sure the state of the chords allows for tritone resolutions in the cases where it is necessary
 * cases handeld for now:
 *      - V65/-> I5 (formula: chords[i] = V & states[i] = 1st) => chords[i+1] = I & states[i+1] = fund)
 *      - V+4->I6 (formula: chords[i] = V & states[i] = 3rd) => chords[i+1] = I & states[i+1] = 1st)
 * todo maybe allow them to go to V/x as well
 * todo test this a lot, for all kinds of variations (also with borrowed chords)
 * @param home the problem space
 * @param size the number of chords
 * @param states the array of chord states
 * @param chords the array of chord degrees
 */
void tritone_resolutions(Home home, int size, IntVarArray states, IntVarArray qualities, IntVarArray chords,
                         IntVarArray bassDegrees);

/**
 * The fifth degree chord cannot be in second inversion if it is not a dominant seventh chord
 * formula: chords[i] == V && qualities[i] < DOMINANT_SEVENTH_CHORD => states[i] != SECOND_INVERSION
 * @param home the problem space
 * @param size the number of chords
 * @param states the array of chord states
 * @param qualities the array of chord qualities
 * @param chords the array of chord degrees
 */
void chord_states_and_qualities(const Home &home, int size, IntVarArray states, IntVarArray qualities);

/***********************************************************************************************************************
 *                                            Optional Constraints (preferences)                                       *
 ***********************************************************************************************************************/

/**
 * Enforces different cadence types at a given position
 * @param home the problem space
 * @param position the position of the first chord of the cadence
 * @param type the type of cadence
 * @param states the array of chord states
 * @param chords the array of chord degrees
 * @param hasSeventh the array of seventh chords
 */
void cadence(const Home &home, int position, int type, IntVarArray states, IntVarArray chords, IntVarArray hasSeventh);

#endif //CHORDGENERATOR_CONSTRAINTS_HPP