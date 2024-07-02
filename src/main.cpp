//
// Created by Damien Sprockeels on 02/07/2024.
//

#include "../headers/ChordGeneratorUtilities.hpp"
#include "../headers/ChordGenerator.hpp"

int main(int argc, char **argv) {
    Tonality* tonality = new MinorTonality(G);
    std::cout << tonality->to_string() << std::endl;

    auto cg = new ChordGenerator(4);

    BAB<ChordGenerator> engine(cg);
    delete cg;
    while(ChordGenerator* cg = engine.next()) {
        std::cout << cg->getChords() << std::endl;
        delete cg;
    }

    return 0;
}