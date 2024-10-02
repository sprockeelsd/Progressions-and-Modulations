//
// Created by Damien Sprockeels on 05/07/2024.
//

#include "../headers/SolveChordGenerator.hpp"

vector<ChordProgression *>
solve_chord_progression_problem(int size, vector<Tonality *> tonalities, vector<int> tonalitiesStarts,
                                vector<int> tonalitiesDurations, vector<int> modulationTypes,
                                vector<int> modulationStarts) {
//    auto cg = new ChordProgression(Home(Gecode::Space()), tonalities, tonalitiesStarts, size, IntVarArray(),
//                                   IntVarArray(), IntVarArray(), IntVarArray(), IntVarArray(),
//                                   0, 1,
//                                   0, 1);
//    std::cout << cg->toString() << std::endl;
//
//    BAB<ChordProgression> engine(cg);
//    delete cg;
//
//    vector<ChordProgression*> sols;
//
//    int n_sols = 0;
//    while(ChordProgression* sol = engine.next()) {
//        if (n_sols >= 1) break;
//        n_sols++;
//        sols.push_back((ChordProgression*) sol->copy());
//        std::cout << "Solution n°" << to_string(n_sols) << ":\n" << sol->toString() << std::endl;
//        delete sol;
//    }
//    std::cout << "Number of solutions: " << n_sols << std::endl;
//    return sols;
    return {};
}

ChordProgression *
solve_chord_progression_problem_best(int size, vector<Tonality *> tonalities, vector<int> tonalitiesStarts,
                                     vector<int> tonalitiesDurations, vector<int> modulationTypes,
                                     vector<int> modulationStarts) {
//    auto sols = solve_chord_progression_problem(size, tonalities, tonalitiesStarts, tonalitiesDurations, modulationTypes,
//                                                modulationStarts);
//    if(sols.empty())
//        return nullptr;
//    return sols.back();
    return nullptr;
}