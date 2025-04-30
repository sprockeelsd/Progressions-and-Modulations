//
// Created by Damien Sprockeels on 02/07/2024.
//

#include "../headers/ChordGeneratorUtilities.hpp"
#include "../Diatony/c++/headers/aux/MidiFileGeneration.hpp"
#include "../headers/TonalPiece.hpp"

//todo move this to diatony in the midifile generation file
void write_midifile(const string& filename, vector<int> notes) {
    int size = notes.size() / 4;
    MidiFile outputFile;
    outputFile.absoluteTicks();     // time information stored as absolute time, will be converted to delta time when written
    outputFile.addTrack(1);   // Add a track to the file (track 0 must be left empty, so add as many as we use

    int tpq = 120;                  // default value in MIDI file is 48 (tempo)
    outputFile.setTicksPerQuarterNote(tpq);

    int actionTime = 0;
    /// array of integers representing the rhythm
    int rhythm[size];
    for(int i = 0; i < size; i++){
        rhythm[i] = 4;
    }

    vector<uchar> midiEvent;        // temporary storage of MIDI events
    midiEvent.resize(3);        //set the size of the array to 3 bites (first bite = Start or end of a note, second bite = note value, third bite = velocity

    /// Fill the MidiFile object
    midiEvent[2] = 64; // store attack/release velocity for note command
    for(int i = 0; i < size; i++){
        midiEvent[0] = 0x90; /// add the start of the note
        for(int j = 0; j < 4; j++){
            midiEvent[1] = notes[4*i+j];
            outputFile.addEvent(1, actionTime, midiEvent);
        }
        actionTime += tpq*rhythm[i]; // increase relative time to the end of these events
        midiEvent[0] = 0x80; /// add the end of the note
        for(int j = 0; j < 4; j++){
            midiEvent[1] = notes[4*i+j];
            outputFile.addEvent(1, actionTime, midiEvent);
        }
    }
    outputFile.sortTracks(); // make sure data is in correct order
    outputFile.write(filename + ".mid");
}

int main(int argc, char **argv) {
    string four_voice = argv[1]; /// true if we want to generate the 4voice chords, false if we just want chords and state
    int size = 20;
    Tonality* Cmajor = new MajorTonality(C);
    Tonality* Cminor = new MinorTonality(C);
    Tonality* Ebmajor = new MajorTonality(E_FLAT);
    Tonality* Bbmajor = new MajorTonality(B_FLAT);
    Tonality* Dmajor = new MajorTonality(D);
    Tonality* Fmajor = new MajorTonality(F);
    Tonality* Gmajor = new MajorTonality(G);

    // int size = 60;
    // vector<Tonality*> tonalities = {Cminor, Ebmajor, Cminor, Cmajor, Gmajor, Cmajor};
    // vector<int> modulationTypes = {PERFECT_CADENCE_MODULATION, PIVOT_CHORD_MODULATION, PERFECT_CADENCE_MODULATION, SECONDARY_DOMINANT_MODULATION, ALTERATION_MODULATION};
    // vector<int> modulationStarts = {12, 18, 27, 39, 48};
    // vector<int> modulationEnds = {13, 22, 28, 40, 50};

    vector<Tonality*> tonalities = {Cminor, Ebmajor};
    vector<int> modulationTypes = {PERFECT_CADENCE_MODULATION};
    vector<int> modulationStarts = {12};
    vector<int> modulationEnds = {13};

    vector<int> phrase_starts = {0, 14};

    auto tonalPiece = new TonalPiece(size, tonalities, modulationTypes,
                                     modulationStarts, modulationEnds);

    /// Optional constraints
    /// Global rules
     // for (auto j = 0; j < tonalities.size(); j++){
     //     auto p = tonalPiece->getChordProgression(j);
     //     for(int i = 0; i < p->getDuration(); i++){
     //         //rel(*this, p->getChords()[i] <= FIVE_OF_SIX);
     //         rel(*tonalPiece, p->getChords()[i] != SEVENTH_DEGREE);
     //         rel(*tonalPiece, p->getChords()[i] != THIRD_DEGREE);
     //     }
     // }

     /// first section
     // rel(*tonalPiece, tonalPiece->getChordProgression(0)->getChords()[0] == FIRST_DEGREE);
     // cadence(*tonalPiece, 7, HALF_CADENCE, tonalPiece->getChordProgression(0)->getStates(), tonalPiece->getChordProgression(0)->getChords(), tonalPiece->getChordProgression(0)->getHasSeventh());
     // rel(*tonalPiece, expr(*tonalPiece, (tonalPiece->getChordProgression(0)->getIsChromatic()[6] == 1)) && expr(*tonalPiece, tonalPiece->getChordProgression(0)->getChords()[6] > FIVE_OF_SEVEN));

     /// Second section

     // rel(*tonalPiece, tonalPiece->getChordProgression(1)->getChords()[0] == FIRST_DEGREE);
     // rel(*tonalPiece, tonalPiece->getChordProgression(1)->getChords()[1] == SECOND_DEGREE);
     // rel(*tonalPiece, tonalPiece->getChordProgression(1)->getChords()[2] == FIFTH_DEGREE_APPOGIATURA);
     // rel(*tonalPiece, tonalPiece->getChordProgression(1)->getChords()[5] == SECOND_DEGREE);
     // rel(*tonalPiece, tonalPiece->getChordProgression(1)->getChords()[6] == FIFTH_DEGREE);
     // //rel(*tonalPiece, tonalPiece->getChordProgression(1)->getChords()[7] != FIFTH_DEGREE);
     // rel(*tonalPiece, tonalPiece->getChordProgression(1)->getChords()[8] == FIVE_OF_SIX);


     // cadence(*tonalPiece, 3, DECEPTIVE_CADENCE, tonalPiece->getChordProgression(1)->getStates(), tonalPiece->getChordProgression(1)->getChords(), tonalPiece->getChordProgression(1)->getHasSeventh());

    //Third section
    // rel(*tonalPiece, tonalPiece->getChordProgression(2)->getChords()[1] == FIRST_DEGREE);


    // for(auto p : progressions){
    //     for(int i = 0; i < p->getDuration(); i++){
    //         rel(*tonalPiece, expr(*tonalPiece,p->getChords()[i] != FIFTH_DEGREE), BOT_IMP, expr(*this, p->getHasSeventh()[i] == 0), true);
    //     }
    //     for(int i = 0; i < p->getDuration()-1; i++){
    //         rel(*tonalPiece, expr(*tonalPiece,p->getChords()[i] != FIFTH_DEGREE || p->getChords()[i] != FIRST_DEGREE),
    //             BOT_IMP, expr(*tonalPiece, p->getChords()[i+1] != p->getChords()[i]), true);
    //     }
    // }



    //rel(*this, progressions[0]->getChords()[2] == AUGMENTED_SIXTH);

    /// For the CPAIOR example

    DFS<TonalPiece> engine(tonalPiece);
    delete tonalPiece;

    int n_sols = 0;
    TonalPiece* last_sol = nullptr;
    auto start = std::chrono::high_resolution_clock::now();     /// start time
    while(TonalPiece* sol = engine.next()) {
        last_sol = sol;
        n_sols += 1;
        if(n_sols >= 1) break;
        delete sol;
    }
    if (n_sols == 0 || last_sol == nullptr) {
        std::cout << "No solution found." << std::endl;
        return 0;
    }
    std::cout << "Number of solutions: " << n_sols << std::endl <<
        "Last solution found:\n" << last_sol->pretty() << std::endl;

    auto end = std::chrono::high_resolution_clock::now();     /// start time
    std::chrono::duration<double> duration = end - start;
    std::cout << "time taken: " << duration.count() << " seconds and " << n_sols << " solutions found.\n" << std::endl;

    std::cout << statistics_to_string(engine.statistics());

    return 0;
}