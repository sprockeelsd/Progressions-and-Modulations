//
// Created by Damien Sprockeels on 07/10/2024.
//

#include "../headers/Modulation.hpp"

Modulation::Modulation(const Home &home, int type, int start, int end, ChordProgression *from, ChordProgression *to):
                        type(type), start(start), end(end), from(from), to(to){

    ///todo post the cst that [start, end-2] is both in t1 and t2, and that [end-2,end] is a PC in t2 (pivot chord modulation, subclass)
    cadence(home, end-1 - to->getStart(), PERFECT_CADENCE, to->getStates(), to->getChords(), to->getHasSeventh()); //todo there is a problem with the indexes this way, they don't correspond between states and chords
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
