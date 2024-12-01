//
// Created by Damien Sprockeels on 30/09/2024.
//

#include "../headers/TonalPiece.hpp"

/**
 * Constructor for TonalPiece objects. It initializes the object with the given parameters, as well as the other
 * objects that post constraints. It also posts the branching for the global arrays. That branching is posted after
 * the ChordProgression objects have posted theirs.
 * @param size the total size of the piece in terms of number of chords
 * @param tonalities a vector of Tonality objects for each tonality of the piece
 * @param tonalitiesStarts a vector of integers representing the starting position of each tonality. They can overlap (modulations)
 * @param tonalitiesDurations a vector of integers representing the duration of each tonality
 * @param modulationTypes a vector of integers representing the type of modulation between the tonalities
 * @param modulationStarts a vector of integers representing the starting position of each modulation
 * @return a TonalPiece object
 */
TonalPiece::
TonalPiece(int size, const vector<Tonality *> &tonalities, vector<int> modulationTypes,
           vector<int> modulationStarts, vector<int> modulationEnds) :
           size(size), tonalities(tonalities),
           modulationTypes(modulationTypes), modulationStarts(modulationStarts), modulationEnds(modulationEnds){

    this->states                = IntVarArray(*this, size, FUNDAMENTAL_STATE,   THIRD_INVERSION);
    this->qualities             = IntVarArray(*this, size, MAJOR_CHORD,         MINOR_MAJOR_SEVENTH_CHORD);
    this->rootNotes             = IntVarArray(*this, size, C,                   B);
    this->hasSeventh            = IntVarArray(*this, size, 0,                   1);
    this->qualityWithoutSeventh = IntVarArray(*this, size, MAJOR_CHORD,         AUGMENTED_CHORD);

    ///constraint
    link_qualities_to_3note_version(*this, size, qualities, qualityWithoutSeventh);

    //todo check alteration modulations, it should last 3 chords because the V might not be directly available. If it is, the third chord is not constrained and is just in the new tonality
    //todo add half diminished chords for the II in minor
    //todo add marche harmoniques (diatoniques/modulantes/chromatiques/par quinte/par quarte/...)
    //todo make it possible to post constraints on the whole piece in the main -> make getters for the tonal piece class
    //todo add V/VII pcq en mineur ça a plus de sens pour les modulations

    //todo add preference for state based on the chord degree (e.g. I should be often used in fund, sometimes 1st inversion, 2nd should be often in 1st inversion, ...)
    //todo add some measure of variety (number of chords used, max % of chord based on degree, ...)

    //todo make an options object that has a field for every parameter
    //todo link with Diatony
    //todo add other chords (9, add6,...)?
    //todo give a range of length for the modulation, so it can have more freedom (extra chords etc)

    if(modulationTypes.size() != tonalities.size()-1)
        throw std::invalid_argument("The number of modulations should be equal to the number of tonalities minus one.");

    ///Compute tonality starts and durations
    tonalitiesStarts.reserve(tonalities.size());
    tonalitiesDurations.reserve(tonalities.size());

    tonalitiesStarts.push_back(0);

    for(int i = 0; i < modulationTypes.size(); i++){
        switch (modulationTypes[i]){
                /**
                 * The modulation lasts 2 chords, and the next tonality starts on the chord after the modulation
                 * example: C Major (I ... V I) (I ...) G Major
                 */
            case PERFECT_CADENCE_MODULATION:    /// The modulation lasts 2 chords, and the next tonality starts on the next chord
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
            case ALTERATION_MODULATION:         /// The modulation lasts 2 chords, and the next tonality starts on the first chord
                tonalitiesStarts        .push_back(this->modulationStarts[i]);
                tonalitiesDurations     .push_back(this->modulationStarts[i] - tonalitiesStarts[i]);
                break;
            case SECONDARY_DOMINANT_MODULATION: /// The modulation lasts 2 chords, and the next tonality starts on the second chord
                tonalitiesStarts        .push_back(this->modulationStarts[i]);
                tonalitiesDurations     .push_back(this->modulationStarts[i] - tonalitiesStarts[i] +1);
                break;
            default:
                throw std::invalid_argument("The modulation type is not recognized.");
        }
    }
    ///last duration
    tonalitiesDurations.push_back(size - tonalitiesStarts[tonalitiesStarts.size()-1]);

    std::cout << "tonalitiesStarts: " << int_vector_to_string(tonalitiesStarts) << std::endl;
    std::cout << "tonalitiesDurations: " << int_vector_to_string(tonalitiesDurations) << std::endl;


    /// Create the ChordProgression objects for each tonality, and post the constraints
    progressions.reserve(tonalities.size());
    for (int i = 0; i < tonalities.size(); i++){
        progressions.push_back(
                new ChordProgression(*this,
                                     tonalitiesStarts[i], tonalitiesDurations[i],
                                     this->tonalities[i],
                                     states, qualities, qualityWithoutSeventh, rootNotes, hasSeventh,
                                     0, 1,
                                     0, 0)
                );
    }

    /// Create the Modulation objects for each modulation, and post the constraints
    modulations.reserve(modulationTypes.size());
    for(int i = 0; i < modulationTypes.size(); i++){
        modulations.push_back(
                new Modulation(*this, modulationTypes[i], modulationStarts[i], modulationEnds[i],
                               progressions[i], progressions[i+1])
                );
    }

    ///add here any optional constraints
    rel(*this, progressions[0]->getChords()[0] == FIRST_DEGREE);

    for (auto p : progressions){
        for(int i = 0; i < p->getDuration(); i++){
            rel(*this, p->getChords()[i] <= FIVE_OF_SIX);
            rel(*this, p->getChords()[i] != SEVENTH_DEGREE);
            rel(*this, p->getChords()[i] != THIRD_DEGREE);
        }
    }

    
    /** The branching on chord degrees is performed first, through the ChordProgression objects.
     * Then it is performed on the global arrays if it is necessary. That means that the branching on degrees is done
     * in order of appearance of the tonalities. Maybe this needs to change? But then it has to be done differently
     * because the chord degrees are not available in this main class. */

    Rnd r(1U);
    for(auto p : progressions)
        branch(*this, p->getChords(), INT_VAR_SIZE_MIN(), INT_VAL_RND(r));
    branch(*this, states,       INT_VAR_SIZE_MIN(), INT_VAL_MIN());
    branch(*this, qualities,    INT_VAR_SIZE_MIN(), INT_VAL_MIN());
//    branch(*this, rootNotes,    INT_VAR_SIZE_MIN(), INT_VAL_MIN());
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
    qualityWithoutSeventh       .update(*this, s.qualityWithoutSeventh);

    for (auto p : s.progressions)
        progressions.push_back(new ChordProgression(*this, *p));

    for (auto m : s.modulations)
        modulations.push_back(new Modulation(*this, *m));
}

/**
 * Returns a string with each of the object's field values as integers. For debugging
 * @brief toString
 * @return a string representation of the object
 */
string TonalPiece::toString() const {
    string txt = "------------------------------------------------------"
                 "TonalPiece object------------------------------------------------------\n";
    txt += "Size: " + to_string(size) + "\n";
    txt += "Tonalities:\t\t";
    for(auto t : tonalities)
        txt += t->get_name() + "\t";
    txt += "\n";
    txt += "Tonalities starts:\t";
    for(auto t : tonalitiesStarts)
        txt += to_string(t) + " ";
    txt += "\n";
    txt += "Tonalities durations:\t";
    for(auto t : tonalitiesDurations)
        txt += to_string(t) + " ";
    txt += "\n";
    txt += "Modulation types:\t";
    for(auto t : modulationTypes)
        txt += to_string(t) + " ";
    txt += "\n";
    txt += "Modulation starts:\t";
    for(auto t : modulationStarts)
        txt += to_string(t) + " ";
    txt += "\n";

    txt += "States:\t\t\t"              + intVarArray_to_string(states) + "\n";
    txt += "Qualities:\t\t"             + intVarArray_to_string(qualities) + "\n";
    txt += "Quality (no seventh):\t"    + intVarArray_to_string(qualityWithoutSeventh) + "\n";
    txt += "Root notes:\t\t"            + intVarArray_to_string(rootNotes) + "\n";
    txt += "Has seventh:\t\t"           + intVarArray_to_string(hasSeventh) + "\n";

    txt += "\nChord Progressions for each tonality:\n";
    for(auto p : progressions)
        txt += p->toString() + "\n\n";
    txt += "\nModulations:\n";
    for(auto m : modulations)
        txt += m->toString() + "\n\n";

    return txt;
}

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
