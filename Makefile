all: run

# Diatony library directories
DIATONY_LIB = Diatony
C++_DIR = c++
SRC_DIR = src
AUX_DIR = aux
MIDI_DIR = midifile
DIATONY_DIR = diatony

#Diatony related files
DIATONY_FILES = 		$(DIATONY_LIB)/$(C++_DIR)/$(SRC_DIR)/$(AUX_DIR)/Utilities.cpp \
						$(DIATONY_LIB)/$(C++_DIR)/$(SRC_DIR)/$(AUX_DIR)/Tonality.cpp \
						$(DIATONY_LIB)/$(C++_DIR)/$(SRC_DIR)/$(AUX_DIR)/MajorTonality.cpp \
						$(DIATONY_LIB)/$(C++_DIR)/$(SRC_DIR)/$(AUX_DIR)/MinorTonality.cpp \
						$(DIATONY_LIB)/$(C++_DIR)/$(SRC_DIR)/$(AUX_DIR)/MidiFileGeneration.cpp \
						$(DIATONY_LIB)/$(C++_DIR)/$(SRC_DIR)/$(DIATONY_DIR)/VoiceLeadingConstraints.cpp	\
						$(DIATONY_LIB)/$(C++_DIR)/$(SRC_DIR)/$(DIATONY_DIR)/HarmonicConstraints.cpp \
						$(DIATONY_LIB)/$(C++_DIR)/$(SRC_DIR)/$(DIATONY_DIR)/GeneralConstraints.cpp \
						$(DIATONY_LIB)/$(C++_DIR)/$(SRC_DIR)/$(DIATONY_DIR)/Preferences.cpp \
						$(DIATONY_LIB)/$(C++_DIR)/$(SRC_DIR)/$(DIATONY_DIR)/FourVoiceTexture.cpp \
						$(DIATONY_LIB)/$(C++_DIR)/$(SRC_DIR)/$(DIATONY_DIR)/SolveDiatony.cpp

MIDI_LIBRARY_FILES = 	$(DIATONY_LIB)/$(C++_DIR)/$(SRC_DIR)/$(MIDI_DIR)/Options.cpp \
						$(DIATONY_LIB)/$(C++_DIR)/$(SRC_DIR)/$(MIDI_DIR)/MidiMessage.cpp \
						$(DIATONY_LIB)/$(C++_DIR)/$(SRC_DIR)/$(MIDI_DIR)/MidiEvent.cpp \
						$(DIATONY_LIB)/$(C++_DIR)/$(SRC_DIR)/$(MIDI_DIR)/MidiEventList.cpp \
						$(DIATONY_LIB)/$(C++_DIR)/$(SRC_DIR)/$(MIDI_DIR)/Binasc.cpp \
						$(DIATONY_LIB)/$(C++_DIR)/$(SRC_DIR)/$(MIDI_DIR)/MidiFile.cpp

CHORD_GENERATOR_FILES = $(SRC_DIR)/ChordGeneratorUtilities.cpp \
						$(SRC_DIR)/Constraints.cpp \
						$(SRC_DIR)/MusicalParts.cpp \
						$(SRC_DIR)/ChordProgression.cpp \
						$(SRC_DIR)/SolveChordGenerator.cpp \
						$(SRC_DIR)/TonalPiece.cpp

compile: clean
	g++ -std=c++11 -F/Library/Frameworks -framework gecode -o out/main \
		$(DIATONY_FILES) $(MIDI_LIBRARY_FILES) $(CHORD_GENERATOR_FILES) $(SRC_DIR)/main.cpp
	install_name_tool -change gecode.framework/Versions/49/gecode /Library/Frameworks/gecode.framework/Versions/49/gecode out/main

run: compile
	./out/main false

4voice: compile
	./out/main true
clean:
	rm -f *.o main