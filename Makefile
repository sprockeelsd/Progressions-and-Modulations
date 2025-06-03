all: run

# import variables from the Diatony repository
include Diatony/c++/file_variables.mk

# add prefix so that the relative path is correct
DIATONY_PATH := Diatony/c++

DIATONY_FILES = $(addprefix $(DIATONY_PATH)/, $(PROBLEM_FILES))
MIDI_LIBRARY_FILES = $(addprefix $(DIATONY_PATH)/, $(MIDI_FILES))

CHORD_GENERATOR_FILES = $(SRC_DIR)/ChordGeneratorUtilities.cpp \
						$(SRC_DIR)/Constraints.cpp \
						$(SRC_DIR)/MusicalParts.cpp \
						$(SRC_DIR)/ChordProgression.cpp \
						$(SRC_DIR)/Modulation.cpp \
						$(SRC_DIR)/TonalPieceParameters.cpp \
						$(SRC_DIR)/TonalPiece.cpp \
						$(SRC_DIR)/HarmoniserSolver.cpp \

compile: clean
	g++ -std=c++11 -F/Library/Frameworks -framework gecode -o out/main \
		$(DIATONY_FILES) $(MIDI_LIBRARY_FILES) $(CHORD_GENERATOR_FILES) $(SRC_DIR)/main.cpp
	install_name_tool -change gecode.framework/Versions/49/gecode /Library/Frameworks/gecode.framework/Versions/49/gecode out/main
	clear

run: compile
	./out/main false

4voice: compile
	./out/main true
clean:
	rm -f *.o main