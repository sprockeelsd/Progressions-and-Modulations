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
TonalPiece:: TonalPiece(int size, const vector<Tonality *> &tonalities, vector<int> modulationTypes,
           vector<int> modulationStarts, vector<int> modulationEnds) :
           size(size), tonalities(tonalities),
           modulationTypes(modulationTypes), modulationStarts(modulationStarts), modulationEnds(modulationEnds){

    this->states                = IntVarArray(*this, size, FUNDAMENTAL_STATE,   THIRD_INVERSION);
    this->qualities             = IntVarArray(*this, size, MAJOR_CHORD,         AUGMENTED_SIXTH_CHORD);
    this->rootNotes             = IntVarArray(*this, size, C,                   B);
    this->hasSeventh            = IntVarArray(*this, size, 0,                   1);
    this->qualitiesWithoutSeventh = IntVarArray(*this, size, MAJOR_CHORD, AUGMENTED_CHORD);

    ///constraint
    link_qualities_to_3note_version(*this, size, qualities, qualitiesWithoutSeventh);

    //todo add control over states (% of fund state, % of inversions,...)
    //todo add preference for state based on the chord degree (e.g. I should be often used in fund, sometimes 1st inversion, 2nd should be often in 1st inversion, ...)
    //todo add some measure of variety (number of chords used, max % of chord based on degree, ...)
    //todo add marche harmoniques (diatoniques/modulantes/chromatiques/par quinte/par quarte/...)
    //todo make an options object that has a field for every parameter
    //todo link with Diatony
    //todo add other chords (9, add6,...)?
    //todo give a range of length for the modulation, so it can have more freedom (extra chords etc)

    if(modulationTypes.size() != tonalities.size()-1)
        throw std::invalid_argument("The number of modulations should be equal to the number of tonalities minus one.");

    ///Compute tonality starts and durations
    tonalitiesStarts.reserve(tonalities.size());    tonalitiesDurations.reserve(tonalities.size());
    /// the first tonality starts at the beginning
    tonalitiesStarts.push_back(0);
    for(int i = 0; i < modulationTypes.size(); i++){
        switch (modulationTypes[i]){
                /**
                 * The modulation lasts 2 chords, and the next tonality starts on the chord after the modulation
                 * example: C Major (I ... V I) (I ...) G Major
                 */
            case PERFECT_CADENCE_MODULATION:
                tonalitiesStarts        .push_back(this->modulationEnds[i] + 1);                       ///start of the next tonality
                tonalitiesDurations     .push_back(this->modulationEnds[i] - tonalitiesStarts[i] + 1); ///duration of the current tonality
                break;
                /**
                 * The modulation lasts at least 3 chords, and the next tonality starts on the first chord while the
                 * first tonality ends just before the perfect cadence, so there is an overlap of the tonalities
                 * example: C Major (I ... (VI) V I ...) G Major
                 */
            case PIVOT_CHORD_MODULATION:
                tonalitiesStarts        .push_back( this->modulationStarts[i]);
                tonalitiesDurations     .push_back(this->modulationEnds[i] -2 - tonalitiesStarts[i] + 1);
                break;
                /**
                 * The modulation lasts 3 chords, and the next tonality starts on the first chord while the first
                 * tonality ends just before the modulation. That is because the V chord might not be possible right
                 * after the alteration.
                 * example: C Major (I ... V I) (IV V ...) F Major
                 */
            case ALTERATION_MODULATION:
                tonalitiesStarts        .push_back(this->modulationStarts[i]);
                tonalitiesDurations     .push_back(this->modulationStarts[i] - tonalitiesStarts[i]);
                break;
                /**
                 * The modulation lasts 2 chords, and the next tonality starts on the first chord while the first
                 * tonality ends on the first chord. There is a 1 chord overlap, that is a secondary dominant in the
                 * first tonality and the V chord in the new tonality.
                 * example: C Major (I ... (V/V) I ...) G Major
                 */
            case SECONDARY_DOMINANT_MODULATION:
                tonalitiesStarts        .push_back(this->modulationStarts[i]);
                tonalitiesDurations     .push_back(this->modulationStarts[i] - tonalitiesStarts[i] +1);
                break;
            default:
                throw std::invalid_argument("The modulation type is not recognized.");
        }
    }
    ///the last section lasts until the end
    tonalitiesDurations.push_back(size - tonalitiesStarts[tonalitiesStarts.size()-1]);

    std::cout << "tonalitiesStarts: " << int_vector_to_string(tonalitiesStarts) << std::endl;
    std::cout << "tonalitiesDurations: " << int_vector_to_string(tonalitiesDurations) << std::endl;

    progressions.reserve(tonalities.size());    modulations.reserve(modulationTypes.size());
    /// Create the ChordProgression objects for each section, and post the constraints
    for (int i = 0; i < tonalities.size(); i++)
        progressions.push_back(
                new ChordProgression(*this, tonalitiesStarts[i], tonalitiesDurations[i],
                                     this->tonalities[i], states, qualities,
                                     qualitiesWithoutSeventh, rootNotes, hasSeventh,
                                     0, 1,
                                     0, 1)
                );

    /// Create the Modulation objects for each modulation, and post the constraints
    for(int i = 0; i < modulationTypes.size(); i++)
        modulations.push_back(
                new Modulation(*this, modulationTypes[i], modulationStarts[i], modulationEnds[i],
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
    size                        = s.size;
    tonalities                  = s.tonalities;
    tonalitiesStarts            = s.tonalitiesStarts;
    tonalitiesDurations         = s.tonalitiesDurations;
    modulationTypes             = s.modulationTypes;
    modulationStarts            = s.modulationStarts;
    modulationEnds              = s.modulationEnds;
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
    txt += "Size: " + to_string(size) + "\n";
    txt += "Tonalities:\t\t";
    for(auto t : tonalities)                  txt += t->get_name() + "\t";                        txt += "\n";
    txt += "Tonalities starts:\t";
    for(auto t : tonalitiesStarts)                  txt += to_string(t) + " ";                     txt += "\n";
    txt += "Tonalities durations:\t";
    for(auto t : tonalitiesDurations)               txt += to_string(t) + " ";                     txt += "\n";
    txt += "Modulation types:\t";
    for(auto t : modulationTypes)                   txt += to_string(t) + " ";                     txt += "\n";
    txt += "Modulation starts:\t";
    for(auto t : modulationStarts)                  txt += to_string(t) + " ";                     txt += "\n";

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
