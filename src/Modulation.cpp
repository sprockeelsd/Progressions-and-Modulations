//
// Created by Damien Sprockeels on 07/10/2024.
//

#include "../headers/Modulation.hpp"

Modulation::Modulation(Home home, int type, int start, int end, ChordProgression *from, ChordProgression *to):
                        type(type), start(start), end(end), from(from), to(to){
    vector<int> t1degreeNotes;
    t1degreeNotes.reserve(SEVENTH_DEGREE+1);
    for(int i = FIRST_DEGREE; i <= SEVENTH_DEGREE; i++)
        t1degreeNotes.push_back(from->getTonality()->get_degree_note(i));
    IntArgs t1Notes(t1degreeNotes);

    vector<int> t1QualitiesDegrees;
    t1QualitiesDegrees.reserve(SEVENTH_DEGREE+1);
    for(int i = FIRST_DEGREE; i <= SEVENTH_DEGREE; i++)
        t1QualitiesDegrees.push_back(from->getTonality()->get_chord_quality(i));
    IntArgs t1Qualities(t1QualitiesDegrees);

    BoolVar isRootNoteInT1(home, 0, 1);
    IntVar degreeInT1(home, FIRST_DEGREE, SEVENTH_DEGREE);
    IntVar qualityInT1(home, MAJOR_CHORD, AUGMENTED_CHORD);

    switch(type){
            /**
             * The first tonality ends on a perfect cadence. Then the next tonality starts
             */
        case PERFECT_CADENCE_MODULATION:
            if(end - start != 1)
                throw std::invalid_argument("A perfect cadence modulation must last exactly 2 chords");
            ///Add a perfect cadence constraint to the end of the first tonality
            cadence(home, from->getDuration()-2, PERFECT_CADENCE, from->getStates(), from->getChords(),
                    from->getHasSeventh());
            break;
            /**
             * A pivot chord (common to both tonalities) is introduced in the first tonality. Then, the rules for both
             * tonalities are applied until there is a perfect cadence in the new tonality
             */
        case PIVOT_CHORD_MODULATION: //todo check that chromatic chords are accepted as well)
            if(end - start < 2)
                throw std::invalid_argument("A pivot chord modulation must last at least 3 chords");
            /// The pivot chord (last from the first tonality and first from the second tonality) must be a diatonic or borrowed chord (not VII)
            rel(home, from->getChords()[from->getDuration()-1] != SEVENTH_DEGREE);
            rel(home, from->getChords()[from->getDuration()-1] <= FIVE_OF_SIX);
            ///The modulation must end on a perfect cadence in the new tonality
            cadence(home, end-1 - to->getStart(), PERFECT_CADENCE, to->getStates(),
                    to->getChords(), to->getHasSeventh());
            break;
            /**
             * The tonality changes by using a chord from the new key that contains a note that is not in the previous key.
             * It must be followed by the V chord in the new tonality
             */
        case ALTERATION_MODULATION:
            if(end - start != 1)
                throw std::invalid_argument("An alteration modulation must last exactly 2 chords");
            /// The first chord of the modulation must be diatonic
            rel(home, from->getChords()[0] <= SIXTH_DEGREE);
            /// Whether the root note of the first chord in the new tonality is in the first tonality
            dom(home, to->getRootNotes()[0], IntSet(t1Notes), isRootNoteInT1);
            /// degreeInT1 is the degree corresponding to the note in the first tonality (might not exist!) todo check how to handle that
            element(home, t1Notes, degreeInT1, to->getRootNotes()[0]);
            //todo link quality and degreeInT1
            element(home, t1Qualities, degreeInT1, qualityInT1);
            element(home, majorDegreeQualities, expr(home, degreeInT1 * nSupportedQualities + qualityInT1), expr(home,!isRootNoteInT1));

            break;
        case SECONDARY_DOMINANT_MODULATION:
            if(end - start != 1)
                throw std::invalid_argument("A secondary dominant modulation must last exactly 2 chords");
            break;
        default:
            throw std::invalid_argument("Invalid modulation type");
    }
}

Modulation::Modulation(const Home &home, const Modulation& m){
    type = m.type;
    start = m.start;
    end = m.end;
    from = new ChordProgression(home, *m.from);
    to = new ChordProgression(home, *m.to);
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
