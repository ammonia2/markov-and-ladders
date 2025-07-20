#pragma once
#include "board.hpp"
#include <vector>

class TransitionMatrix {
    std::vector<std::vector<double>> matrix;
    std::vector<std::vector<BoardEntity*>> board;
    int totalStates;

    public:
    TransitionMatrix(std::vector<std::vector<BoardEntity*>> b, int s)
        : board(b), totalStates(s), 
        // initialised with 0
        matrix(s, std::vector<double>(totalStates, 0)) {}
};