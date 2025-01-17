//
// Created by Damien Sprockeels on 02/07/2024.
//

#include "../headers/ChordGeneratorUtilities.hpp"
#include "../Diatony/c++/headers/aux/MidiFileGeneration.hpp"
#include "../headers/TonalPiece.hpp"

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
    int size = 10;
    Tonality* Cmajor = new MajorTonality(C);
    Tonality* Gmajor = new MajorTonality(G);
    vector<Tonality*> tonalities = {Cmajor, Gmajor};
    vector<int> modulationTypes = {SECONDARY_DOMINANT_MODULATION};
    vector<int> modulationStarts = {4};
    vector<int> modulationEnds = {5};
    /// For the CPAIOR example
//    vector<Tonality*> tonalities = {Ebmajor, Bbmajor};
//    vector<int> modulationTypes = {PIVOT_CHORD_MODULATION};
//    vector<int> modulationStarts = {4};
//    vector<int> modulationEnds = {8};

    auto tonalPiece = new TonalPiece(size, tonalities, modulationTypes,
                                     modulationStarts, modulationEnds);

    /// Optional constraints

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

    /// Generate voicing using Diatony
    for (int cp = 0; cp < tonalities.size(); cp++) {
        auto progression = last_sol->getChordProgression(cp);
        auto voicing = solve_diatony_problem_optimal(
            last_sol->getTonalityDuration(cp),
            last_sol->getTonality(cp),
            IntVarArray_to_int_vector(progression->getChords()),
            IntVarArray_to_int_vector(progression->getQualities()),
            IntVarArray_to_int_vector(progression->getStates()));
        write_midifile("out/MidiFiles/section" + to_string(cp), intVarArray_to_int_vector(voicing->getFullVoicing()));
        std::cout << "midifile created" << std::endl;
    }

    return 0;
}