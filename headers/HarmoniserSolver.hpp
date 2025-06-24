//
// Created by Damien Sprockeels on 16/05/2025.
//

#ifndef HARMONISERSOLVER_HPP
#define HARMONISERSOLVER_HPP

#include "TonalPiece.hpp"

/**
 * Solves a harmonization problem for a given TonalPiece.
 * @param piece the TonalPiece to solve
 * @param print if true, prints the number of solutions and the last solution found
 * @return the last solution found, or nullptr if no solution was found
 */
TonalPiece* solve_harmoniser(TonalPiece* piece, bool print = false);

#endif //HARMONISERSOLVER_HPP
