//
// Created by Damien Sprockeels on 07/10/2024.
//

#ifndef CHORDGENERATOR_MODULATION_HPP
#define CHORDGENERATOR_MODULATION_HPP

#include "ChordGeneratorUtilities.hpp"
#include "ChordProgression.hpp"

/**
 * This class represents a modulation between two tonalities. It takes as argument a search space, a type of modulation,
 * a start position, and end position as well as two chord progressions to modulate between.
 *
 * It posts constraints based on the type of modulation on the variables from the chord progressions.
 */
class Modulation {
private:
    int type;                   /// the type of modulation
    int start;                  /// the starting position of the modulation
    int end;                    /// the ending position of the modulation

    ChordProgression* from;     /// the chord progression to modulate from
    ChordProgression* to;       /// the chord progression to modulate to

public:
    /**
     * Constructor for Modulation objects. It initializes the object with the given parameters, and posts the
     * constraints based on the type of modulation.
     * @param home the search space
     * @param type the type of modulation
     * @param start the starting position of the modulation
     * @param end the ending position of the modulation
     * @param from the chord progression to modulate from
     * @param to the chord progression to modulate to
     */
    Modulation(Home home, int type, int start, int end, ChordProgression *from, ChordProgression *to);

    /**
     * Copy constructor
     * @param home the search space
     * @param s a Modulation object
     */
    Modulation(const Home &home, const Modulation& m);

    /**
     * This function posts the constraints for a perfect cadence modulation. It ensures that the first chord progression
     * ends on a perfect cadence.
     * @param home the search space
     */
    void perfect_cadence_modulation(const Home &home);

    /**
     * This function posts the constraints for a pivot chord modulation. It ensures that from the start of the modulation
     * to the end minus two chords, all chords are in both tonalities. The last two chords of the modulation are a perfect
     * cadence in the new tonality
     * @param home the search space
     */
    void pivot_chord_modulation(const Home &home);

    void alteration_modulation(Home home);

    void secondary_dominant_modulation(Home home);

    string toString();

    string pretty();
};

#endif //CHORDGENERATOR_MODULATION_HPP
