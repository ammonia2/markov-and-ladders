#include <iostream>
#include <ctime>
#include <random>
#include <vector>

#include "boardEntity.hpp"
#include "board.hpp"
#include "transitionMatrix.hpp"
using namespace std;

int rollDice() {
    // generates a random number in range 1-6
    static random_device rd; // don't need reinitialisation everytime
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

    TransitionMatrix pMatrix(board->getBoard(), boardLength*boardHeight, boardLength);
    pMatrix.calculateProbabilities();
    // matrix.exportToCSV("dataset.csv");

    Matrix<double> fundamentalMatrix = pMatrix.getFundamentalMatrix();

    // ANALYSIS: expected total moves to win from starting position
    int numStates = fundamentalMatrix.getRows();
    Matrix<double> ones(numStates, 1, 1.0);
    Matrix<double> expectedMoves = fundamentalMatrix* ones;

    double avgGameLength = expectedMoves[0][0];
    cout << "Expected moves from start: " << avgGameLength << endl;
}