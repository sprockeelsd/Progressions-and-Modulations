//
// Created by Damien Sprockeels on 02/07/2024.
//

#include "../headers/ChordGeneratorUtilities.hpp"
#include "../headers/ChordGenerator.hpp"

int main(int argc, char **argv) {
    Tonality* Gmajor = new MajorTonality(G);

    auto cg = new ChordGenerator(4, Gmajor);

    BAB<ChordGenerator> engine(cg);
    delete cg;

    int n_sols = 1;
    while(ChordGenerator* sol = engine.next()) {
        //if(n_sols == 10) break;
        std::cout << "Solution n°" << to_string(n_sols) << ":\n" << sol->pretty() << std::endl;
        delete sol;
        n_sols++;
    }
    std::cout << "Number of solutions: " << n_sols << std::endl;
    return 0;
}