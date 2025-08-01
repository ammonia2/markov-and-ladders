#include <iostream>
#include <ctime>
#include <random>
#include <vector>

#include "boardEntity.hpp"
#include "board.hpp"
#include "transitionMatrix.hpp"
#include "matplotlibcpp.h"

namespace plt = matplotlibcpp;
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
    int totalStates = boardHeight*boardLength;

    Board *board = new Board(snakesCount, ladderCount, boardLength, boardHeight);
    board->displayBoard();

    TransitionMatrix pMatrix(board->getBoard(), boardLength*boardHeight, boardLength);
    pMatrix.calculateProbabilities();
    // matrix.exportToCSV("dataset.csv");

    Matrix<double> fundamentalMatrix = pMatrix.getFundamentalMatrix();

    // ANALYSIS: expected total moves to win from each block
    int numStates = fundamentalMatrix.getRows();
    Matrix<double> ones(numStates, 1, 1.0);
    Matrix<double> expectedMoves = fundamentalMatrix * ones;

    // plotting
    vector<double> blocks;
    vector<double> expectedMovesVec;
    for (int i = 0; i < numStates; ++i) {
        blocks.push_back(i + 1); // Block numbers (1-indexed)
        expectedMovesVec.push_back(expectedMoves[i][0]);
    }

    plt::figure();
    plt::plot(blocks, expectedMovesVec);
    plt::title("Expected Moves to Win from Each Block");
    plt::xlabel("Board Block");
    plt::ylabel("Expected Moves");
    plt::grid(true);
    plt::show();

    // double avgGameLength = expectedMoves[0][0];
    // cout << "Expected moves to win from start: " << avgGameLength << endl;

    // ANALYSIS: probability of being on each square after K turns
    Matrix<double> P = pMatrix.getTransitionMatrix();

    Matrix<double> initialDistribution (totalStates, 1, 0.0);
    initialDistribution[0][0] = 1.0; // always starts from square 1 on the board

    vector<double> steps;
    for (int i = 0; i < 100; i++) {
        steps.push_back(static_cast<double>(i));
    }

    vector<double> winningProbs;

    for (int k: steps) {
        // cout<<"\nAfter "<<k<<" turns"<<endl;

        Matrix<double> currentDistribution = initialDistribution;
        for (int turn = 0; turn < k; turn++)
            currentDistribution = P.transpose() * currentDistribution;

        double winProbability = currentDistribution[totalStates -1][0];
        winningProbs.push_back(winProbability);
        // cout<<"Probability of winning: "<<winProbability<<endl;
        
        // most likely position (excluding absorbing state)
        int mostLikelyPos = 0;
        double maxProb = currentDistribution[0][0];
        
        for (int i = 1; i < totalStates-1; i++) {
            if (currentDistribution[i][0] > maxProb) {
                maxProb = currentDistribution[i][0];
                mostLikelyPos = i;
            }
        }

        // cout<<"Most likely position: "<<mostLikelyPos + 1<<" (probability: "<<maxProb<<")"<<endl;
    }

    plt::figure();
    plt::plot(steps, winningProbs);
    plt::title("Winning Probability after steps");
    plt::xlabel("Number of Steps");
    plt::ylabel("Winning Probability");
    plt::grid(true);
    plt::show();

}