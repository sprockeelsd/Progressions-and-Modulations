//
// Created by Damien Sprockeels on 16/05/2025.
//

#include "../headers/HarmoniserSolver.hpp"

TonalPiece* solve_harmoniser(TonalPiece* piece, bool print) {

    DFS<TonalPiece> engine(piece);
    delete piece;

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
        if (print) std::cout << "No solution found." << std::endl;
        return nullptr;
    }
    if (print) std::cout << "Number of solutions: " << n_sols << std::endl <<
        "Last solution found:\n" << last_sol->pretty() << std::endl;

    auto end = std::chrono::high_resolution_clock::now();     /// start time
    std::chrono::duration<double> duration = end - start;
    if (print) std::cout << "time taken: " << duration.count() << " seconds and " << n_sols << " solutions found.\n" << std::endl;

    if (print) std::cout << statistics_to_string(engine.statistics());
    return last_sol;
}
