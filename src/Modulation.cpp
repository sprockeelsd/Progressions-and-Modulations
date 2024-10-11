//
// Created by Damien Sprockeels on 07/10/2024.
//

#include "../headers/Modulation.hpp"

Modulation::Modulation(const Home &home, int type, int start, int end, ChordProgression *from, ChordProgression *to):
                        type(type), start(start), end(end), from(from), to(to){

    switch(type){
            /**
             * The first tonality ends on a perfect cadence. Then the next tonality starts
             */
        case PERFECT_CADENCE_MODULATION:
            if(end - start != 1)
                throw std::invalid_argument("A perfect cadence modulation must last exactly 2 chords");
            ///Add a perfect cadence constraint to the end of the first tonality
            cadence(home, start, PERFECT_CADENCE, from->getStates(), from->getChords(),
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
        case ALTERATION_MODULATION:
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
