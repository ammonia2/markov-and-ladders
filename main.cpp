#include <iostream>
#include <ctime>
#include <random>
#include <vector>

#include "ladder.hpp"
#include "snake.hpp"
#include "boardEntity.hpp"
#include "board.hpp"
#include "matrix.hpp"
using namespace std;

int rollDice() {
    // generates a random number in range 1-6
    static random_device rd; // dont need reinitialisation everytime
    static mt19937 gen(rd());
    uniform_int_distribution<> dis(1, 6);

    int number = dis(gen);

    return number;
}

int main() {
    // declaring constants for initialising a random Snake & Ladder board
    const int snakesCount = 8 + rand() % 4, ladderCount = 8 + rand() % 4;
    const int boardLength = 10, boardHeight = 10;

    Board *board = new Board(snakesCount, ladderCount, boardLength, boardHeight);
    board->displayBoard();

    TransitionMatrix matrix(board->getBoard(), boardLength*boardHeight, boardLength);
    matrix.calculateProbabilities();
    matrix.exportToCSV("dataset.csv");
}