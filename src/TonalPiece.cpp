//
// Created by Damien Sprockeels on 30/09/2024.
//

#include "../headers/TonalPiece.hpp"

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
TonalPiece:: TonalPiece(TonalPieceParameters* params) : parameters(params) {

    this->states                = IntVarArray(*this, params->get_size(), FUNDAMENTAL_STATE,   THIRD_INVERSION);
    this->qualities             = IntVarArray(*this, params->get_size(), MAJOR_CHORD,         MINOR_NINTH_DOMINANT_CHORD);
    this->rootNotes             = IntVarArray(*this, params->get_size(), C,                   B);
    this->hasSeventh            = IntVarArray(*this, params->get_size(), 0,                   1);
    this->qualitiesWithoutSeventh = IntVarArray(*this, params->get_size(), MAJOR_CHORD, AUGMENTED_CHORD); //todo modify this since augmented sixth are also 3 note chords

    ///constraint
    link_qualities_to_3note_version(*this, params->get_size(), qualities, qualitiesWithoutSeventh);

    //todo add control over states (% of fund state, % of inversions,...)
    //todo add preference for state based on the chord degree (e.g. I should be often used in fund, sometimes 1st inversion, 2nd should be often in 1st inversion, ...)
    //todo add some measure of variety (number of chords used, max % of chord based on degree, ...)
    //todo add marche harmoniques (diatoniques/modulantes/chromatiques/par quinte/par quarte/...)
    //todo make an options object that has a field for every parameter
    //todo link with Diatony
    //todo add other chords (9, add6,...)?
    //todo give a range of length for the modulation, so it can have more freedom (extra chords etc)

    progressions.reserve(params->get_nProgressions());    modulations.reserve(params->get_nProgressions() - 1);
    /// Create the ChordProgression objects for each section, and post the constraints
    for (int i = 0; i < params->get_nProgressions(); i++)
        progressions.push_back(
                new ChordProgression(*this, parameters->get_progressionStart(i), parameters->get_progressionDuration(i),
                                     parameters->get_tonality(i), states, qualities,
                                     qualitiesWithoutSeventh, rootNotes, hasSeventh,
                                     0, 1,
                                     0, 1)
                );

    /// Create the Modulation objects for each modulation, and post the constraints
    for(int i = 0; i < params->get_nProgressions() - 1; i++)
        modulations.push_back(
        new Modulation(*this, parameters->get_modulationType(i), params->get_modulationStart(i), parameters->get_modulationEnd(i),
                       progressions[i], progressions[i+1])
        );


    /** The branching on chord degrees is performed first, through the ChordProgression objects. Then it is performed
     * on state and quality if necessary.*/

    Rnd r(1U);
    for(auto p : progressions)
        branch(*this, p->getChords(), INT_VAR_SIZE_MIN(), INT_VAL_RND(r));
    branch(*this, states,       INT_VAR_SIZE_MIN(), INT_VAL_MIN());
    branch(*this, qualities,    INT_VAR_SIZE_MIN(), INT_VAL_MIN());
}

/**
 * @brief Copy constructor
 * @param s a ChordProgression object pointer
 * Returns a TonalPiece object that is equivalent to s
 */
TonalPiece::TonalPiece(TonalPiece &s) : Space(s){
    parameters                  = s.parameters;
    states                      .update(*this, s.states);
    qualities                   .update(*this, s.qualities);
    rootNotes                   .update(*this, s.rootNotes);
    hasSeventh                  .update(*this, s.hasSeventh);
    qualitiesWithoutSeventh       .update(*this, s.qualitiesWithoutSeventh);

    for (auto p : s.progressions)
        progressions.push_back(new ChordProgression(*this, *p));

    for (auto m : s.modulations)
        modulations.push_back(new Modulation(*this, *m));
}

/**
 * Returns a string with each of the object's field values as integers.
 * @brief toString
 * @return a string representation of the object
 */
string TonalPiece::toString() const {
    string txt = "------------------------------------------------------TonalPiece object------------------------------"
                 "------------------------\n";
    txt += "Parameters: n" + parameters->toString();

    txt += "States:\t\t\t"                  + intVarArray_to_string(states)                         + "\n";
    txt += "Qualities:\t\t"                 + intVarArray_to_string(qualities)                      + "\n";
    txt += "Quality (no seventh):\t"        + intVarArray_to_string(qualitiesWithoutSeventh)        + "\n";
    txt += "Root notes:\t\t"                + intVarArray_to_string(rootNotes)                      + "\n";
    txt += "Has seventh:\t\t"               + intVarArray_to_string(hasSeventh)                     + "\n";

    txt += "\nChord Progressions for each tonality:\n";
    for(auto p : progressions)      txt += p->toString()                                    + "\n\n";
    txt += "\nModulations:\n";
    for(auto m : modulations)            txt += m->toString()                                    + "\n\n";

    return txt;
}

/**
 * Returns a string representing the piece in a prettier format, more readable.
 * @brief pretty function
 * @return a string representation of the object
 */
string TonalPiece::pretty() const {
    string txt;
    for(int i = 0; i < progressions.size(); i++){
        txt += progressions[i]->pretty() + "\n";
        if(i < modulations.size())
            txt += modulations[i]->pretty() + "\n";
        txt += "\n";
    }
    return txt;
}

/**
 * @brief copy function
 * @return a Space* object that is a copy of the current object
 */
Space* TonalPiece::copy() {
    return new TonalPiece(*this);
}
