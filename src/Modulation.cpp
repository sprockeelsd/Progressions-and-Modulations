//
// Created by Damien Sprockeels on 07/10/2024.
//

#include "../headers/Modulation.hpp"

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
Modulation::Modulation(Home home, int type, int start, int end, ChordProgression *from, ChordProgression *to):
                        type(type), start(start), end(end), from(from), to(to){
    switch(type){
            /**
             * The first tonality ends on a perfect cadence. Then the next tonality starts
             */
        case PERFECT_CADENCE_MODULATION:
            if(end - start != 1)
                throw std::invalid_argument("A perfect cadence modulation must last exactly 2 chords");
            perfect_cadence_modulation(home);
            break;
            /**
             * A pivot chord (common to both tonalities) is introduced in the first tonality. Then, the rules for both
             * tonalities are applied until there is a perfect cadence in the new tonality
             */
        case PIVOT_CHORD_MODULATION: //todo check that chromatic chords are accepted as well)
            if(end - start < 2)
                throw std::invalid_argument("A pivot chord modulation must last at least 3 chords");
            pivot_chord_modulation(home);
            break;
            /**
             * The tonality changes by using a chord from the new key that contains a note that is not in the previous key.
             * It must be followed by the V chord in the new tonality todo make it so that the V is the second or third chord in the new tonality
             */
        case ALTERATION_MODULATION:
            if(end - start != 1)
                throw std::invalid_argument("An alteration modulation must last exactly 2 chords");
            alteration_modulation(home);
            break;
        case SECONDARY_DOMINANT_MODULATION:
            if(end - start != 1)
                throw std::invalid_argument("A secondary dominant modulation must last exactly 2 chords");
            secondary_dominant_modulation(home);
            break;
        default:
            throw std::invalid_argument("Invalid modulation type");
    }
}

/**
 * Copy constructor
 * @param home the search space
 * @param s a Modulation object
 */
Modulation::Modulation(const Home &home, const Modulation& m){
    type = m.type;
    start = m.start;
    end = m.end;
    from = new ChordProgression(home, *m.from);
    to = new ChordProgression(home, *m.to);
}

/**
 * This function posts the constraints for a perfect cadence modulation. It ensures that the first chord progression
 * ends on a perfect cadence.
 * @param home the search space
 */
void Modulation::perfect_cadence_modulation(const Home &home) {
    ///Add a perfect cadence constraint to the end of the first tonality
    cadence(home, from->getDuration()-2, PERFECT_CADENCE, from->getStates(), from->getChords(),
            from->getHasSeventh());
}

/**
 * This function posts the constraints for a pivot chord modulation. It ensures that from the start of the modulation
 * to the end minus two chords, all chords are in both tonalities. The last two chords of the modulation are a perfect
 * cadence in the new tonality
 * @param home the search space
 */
void Modulation::pivot_chord_modulation(const Home &home) {
    /// The pivot chord (last from the first tonality and first from the second tonality) must be a diatonic or borrowed chord (not VII)
    //todo this is wrong, the pivot chord is not the last chord of the first tonality, but the first chord of the second tonality. It has to be a diatonic chord in the first tonality
    int mod_start_in_from = start - from->getStart();
    rel(home, from->getChords()[mod_start_in_from] != SEVENTH_DEGREE); //todo check pb V/II into Vda
    ///The modulation must end on a perfect cadence in the new tonality
    cadence(home, end-1 - to->getStart(), PERFECT_CADENCE, to->getStates(),
            to->getChords(), to->getHasSeventh());
}

void Modulation::alteration_modulation(Home home) {
    /// The last chord of the first tonality must be diatonic and not the seventh degree without a seventh
    rel(home, from->getChords()[from->getDuration()-1] < SEVENTH_DEGREE);
    rel(home, from->getHasSeventh()[from->getDuration()-1] == 0);

    /// The first chord of the modulation must be diatonic and not the fifth degree. It cannot have a seventh
    rel(home, to->getChords()[0] <= SEVENTH_DEGREE);
    rel(home, to->getChords()[0] != FIFTH_DEGREE);
    rel(home, to->getHasSeventh()[0] == 0);

    /// Get the diatonic note for each degree in the first tonality. Then, add the other notes in the end (they don't
    /// have a degree, but it is useful to post the constraint). This way all notes are in the array, and we can use an
    /// element constraint
    vector<int> t1degreeNotes;      t1degreeNotes.reserve(PERFECT_OCTAVE);
    for(int i = FIRST_DEGREE; i <= SEVENTH_DEGREE; i++) { /// Add the degree notes
        t1degreeNotes.push_back(from->getTonality()->get_degree_note(i));
    }
    for(int i = C; i <= B; i++){ /// Add the other notes
        /// if the note is not in the array
        if(std::find(t1degreeNotes.begin(), t1degreeNotes.end(), i) == t1degreeNotes.end()){
            t1degreeNotes.push_back(i); /// add it at the end
        }
    }
    IntArgs t1Notes(t1degreeNotes); /// Create the argument array for the constraint

    /// Get the quality associated to each degree in the first tonality. Then, add -1 for every note that is not associated
    /// to a degree in the tonality. This way, we can use an element constraint to link the degree and the quality
    vector<int> t1QualitiesDegrees;     t1QualitiesDegrees.reserve(PERFECT_OCTAVE);
    for(int i = FIRST_DEGREE; i <= SEVENTH_DEGREE; i++) { /// Add the qualities for the degrees
        t1QualitiesDegrees.push_back(from->getTonality()->get_chord_quality(i));
    }
    while(t1QualitiesDegrees.size() < t1degreeNotes.size()) {
        t1QualitiesDegrees.push_back(-1); /// fake qualities to have the right number of elements
    }
    IntArgs t1Qualities(t1QualitiesDegrees); /// Create the argument array for the constraint

    /// The corresponding degree and quality in T1 for the note in the new tonality
    IntVar degreeInT1(home, FIRST_DEGREE, PERFECT_OCTAVE - 1); /// If it is not in the tonality, it is above the seventh degree
    IntVar qualityInT1(home, -1, AUGMENTED_CHORD); /// Simplified quality without the seventh in T1

    /// degreeInT1 is the degree corresponding to the note in the first tonality. If it does not exist,
    /// it has a fake degree value (above seventh degree)
    element(home, t1Notes, degreeInT1, expr(home, to->getRootNotes()[0] % PERFECT_OCTAVE));
    /// link quality and degreeInT1. If the degree is fake, the quality is -1
    element(home, t1Qualities, degreeInT1, qualityInT1);
    /// the quality of the chord in the new tonality cannot be the same as the quality for the same note in t1.
    /// if the note is not in t1, it is always true because quality is -1. Otherwise the constraint is enforced.
    rel(home, qualityInT1 != to->getQualitiesWithoutSeventh()[0]);
    //element(home, majorDegreeQualities, expr(home, degreeInT1 * nSupportedQualities + qualityInT1), expr(home,!isRootNoteInT1)); old version, not working

    //todo some chords cannot be followed by the V, so it should be the third chord. Depending on the first chord, it should be as soon as possible but a chord that can be followed by V directly is not better than one that needs a transition chord to V
    BoolVar canNextChordBeV(home, 0, 1); /// If the next chord can be the V chord
    element(home, tonalTransitions, expr(home, to->getChords()[0] * nSupportedChords + FIFTH_DEGREE), canNextChordBeV);
    /// If the next chord can be the V, then it is
    rel(home, canNextChordBeV, BOT_EQV, expr(home, to->getChords()[1] == FIFTH_DEGREE), true);
    /// If the next chord cannot be the V, then the third chord is
    rel(home, expr(home, !canNextChordBeV), BOT_IMP, expr(home, to->getChords()[2] == FIFTH_DEGREE), true);
}

/// The note below the leading tone of the new tonality must be present in the chord before the V, which is the first chord of the new tonality
//todo make an overlap between the tonalities that is the secondary dominant chord
void Modulation::secondary_dominant_modulation(const Home& home) {
    rel(home, to->getChords()[0] == FIFTH_DEGREE); /// The first chord of the new tonality must be the V chord

    /// calculer la distance entre les tonalités ( abs(dest - orig) % 7),
    int tonics_interval = abs(to->getTonality()->get_tonic() - from->getTonality()->get_tonic()) % PERFECT_OCTAVE;
    int absolute_tonics_interval = 0;
    switch (tonics_interval){
        case MINOR_SECOND:
        case MAJOR_SECOND:
            absolute_tonics_interval = 1;
            break;
        case MINOR_THIRD:
        case MAJOR_THIRD:
            absolute_tonics_interval = 2;
            break;
        case PERFECT_FOURTH:
            absolute_tonics_interval = 3;
            break;
        case PERFECT_FIFTH:
            absolute_tonics_interval = 4;
            break;
        case MINOR_SIXTH:
        case MAJOR_SIXTH:
            absolute_tonics_interval = 5;
            break;
        case MINOR_SEVENTH:
        case MAJOR_SEVENTH:
            absolute_tonics_interval = 6;
            break;
        default:
            break;
    }
    //std::cout << "absolute tonic intervals: " << absolute_tonics_interval << std::endl;

    int degree_of_new_seventh_in_from = (absolute_tonics_interval + 6) % 7;
    //std::cout << "degree of new seventh in from: " << degree_of_new_seventh_in_from << std::endl;

    /// this degree must be in the last chord of the first tonality
    rel(home, expr(home, from->getRoots()[from->getDuration()-2] == degree_of_new_seventh_in_from ||
                            from->getThirds()[from->getDuration()-2] == degree_of_new_seventh_in_from ||
                            from->getFifths()[from->getDuration()-2] == degree_of_new_seventh_in_from));
}

string Modulation::toString() {
    string txt;
    txt += "------------------------Modulation object------------------------\n";
    txt += "Type: " + to_string(type) + "\n";
    txt += "Start: " + to_string(start) + "\n";
    txt += "End: " + to_string(end) + "\n";
    txt += "From: " + from->getTonality()->get_name() + "\n";
    txt += "To: " + to->getTonality()->get_name() + "\n";

    return txt;
}

string Modulation::pretty() {
    string txt;
    try{
        txt += "from " + from->getTonality()->get_name() + " to " + to->getTonality()->get_name() + " (" + modulation_type_names[type] + ")";
    }
    catch(exception& e){
        std::cout << "Some variables are unbound in the modulation object" << std::endl;
    }
    return txt;
}
