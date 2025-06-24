//
// Created by Damien Sprockeels on 07/10/2024.
//

#ifndef CHORDGENERATOR_MODULATION_HPP
#define CHORDGENERATOR_MODULATION_HPP

#include "ChordGeneratorUtilities.hpp"
#include "ChordProgression.hpp"

/**
 * This class represents a modulation between two progressions in two tonalities. It takes as argument a search space, a type of modulation,
 * a start position, and end position as well as two chord progressions to modulate between.
 * It posts constraints based on the type of modulation on the variables from the chord progressions.
 */
class Modulation {
private:
    int type;       int start;      int end;

    ChordProgression* from;    ChordProgression* to;

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
    Modulation(const Home& home, int type, int start, int end, ChordProgression *from, ChordProgression *to);

    /**
     * Copy constructor
     * @param home the search space
     * @param m a Modulation object
     */
    Modulation(const Home &home, const Modulation& m);

    /**
     * This function posts the constraints for a perfect cadence modulation. It ensures that the first chord progression
     * ends in a perfect cadence.
     * @param home the search space
     */
    void perfect_cadence_modulation(const Home &home) const;

    /**
     * This function posts the constraints for a pivot chord modulation. It ensures that from the start of the modulation
     * to the end minus two chords, all chords are in both tonalities. The last two chords of the modulation are a perfect
     * cadence in the new tonality
     * @param home the search space
     */
    void pivot_chord_modulation(const Home &home) const;

    /**
     * This function posts the constraints for an alteration modulation. It ensures that the first tonality ends on a diatonic
     * chord, and that the first chord of the new tonality contains at least one note not in the first tonality. It also
     * enforces that the V chord of the new tonality must be at the second or third position in the new tonality's section.
     * @param home the search space
     */
    void alteration_modulation(Home home) const;

    /**
     * This function posts the constraints for a secondary dominant modulation. It ensures that the first chord of the new tonality
     * is the V chord, and that the last chord of the first tonality contains the note below the leading tone of the new tonality.
     * @param home the search space
     */
    void secondary_dominant_modulation(const Home& home) const;

    /**
     * Returns a string with each of the object's field values as integers.
     * @return
     */
    string toString() const;

    /**
     * Returns a string representing the piece in a prettier format, more readable.
     * @return
     */
    string pretty() const;
};

#endif //CHORDGENERATOR_MODULATION_HPP
