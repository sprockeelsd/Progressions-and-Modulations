//
// Created by Damien Sprockeels on 05/07/2024.
//

#include "../headers/SolveChordGenerator.hpp"

vector<ChordGenerator*> solve_chord_progression_problem(int size, Tonality* tonality){
    auto cg = new ChordGenerator(size, tonality, 0.25);

    BAB<ChordGenerator> engine(cg);
    delete cg;

    vector<ChordGenerator*> sols;

    int n_sols = 0;
    while(ChordGenerator* sol = engine.next()) {
        //if (n_sols >= 10) break;
        n_sols++;
        sols.push_back((ChordGenerator*) sol->copy());
        std::cout << "Solution n°" << to_string(n_sols) << ":\n" << sol->pretty() << std::endl;
        delete sol;
    }
    std::cout << "Number of solutions: " << n_sols << std::endl;
    return sols;
}

ChordGenerator* solve_chord_progression_problem_best(int size, Tonality* tonality){
    auto sols = solve_chord_progression_problem(size, tonality);
    if(sols.empty())
        return nullptr;
    return sols.back();
}