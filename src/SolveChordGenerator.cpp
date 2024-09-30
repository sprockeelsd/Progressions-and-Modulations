//
// Created by Damien Sprockeels on 05/07/2024.
//

#include "../headers/SolveChordGenerator.hpp"

vector<ChordGenerator *>
solve_chord_progression_problem(int size, vector<Tonality *> tonalities, vector<int> tonalitiesStarts,
                                vector<int> tonalitiesDurations, vector<int> modulationTypes,
                                vector<int> modulationStarts) {
    auto cg = new ChordGenerator(size, tonalities, tonalitiesStarts, tonalitiesDurations, modulationTypes, modulationStarts,
                                 0, 1,
                                 0, 1);
    std::cout << cg->toString() << std::endl;

    BAB<ChordGenerator> engine(cg);
    delete cg;

    vector<ChordGenerator*> sols;

    int n_sols = 0;
    while(ChordGenerator* sol = engine.next()) {
        if (n_sols >= 1) break;
        n_sols++;
        sols.push_back((ChordGenerator*) sol->copy());
        std::cout << "Solution n°" << to_string(n_sols) << ":\n" << sol->toString() << std::endl;
        delete sol;
    }
    std::cout << "Number of solutions: " << n_sols << std::endl;
    return sols;
}

ChordGenerator *
solve_chord_progression_problem_best(int size, vector<Tonality *> tonalities, vector<int> tonalitiesStarts,
                                     vector<int> tonalitiesDurations, vector<int> modulationTypes,
                                     vector<int> modulationStarts) {
    auto sols = solve_chord_progression_problem(size, tonalities, tonalitiesStarts, tonalitiesDurations, modulationTypes,
                                                modulationStarts);
    if(sols.empty())
        return nullptr;
    return sols.back();
}