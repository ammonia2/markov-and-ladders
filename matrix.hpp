#pragma once
#include "board.hpp"
#include <vector>
#include <fstream>

class TransitionMatrix {
    /* matrix is a 2D vector of size totalStates x totalStates
     this is a representation of the first order markov model
     where each state has a probability of transitioning to every
     other state*/
    std::vector<std::vector<double>> matrix;
    std::vector<std::vector<BoardEntity*>> board;
    int boardLength;
    int totalStates;

    std::pair<int, int> nonLineariseBlock(int block) {
        int row = block / boardLength;
        int col = block % boardLength;
        
        return std::make_pair(row, col);
    }

    void calculateTransitionProbs(int block) {
        // for every possible dice state (0 - 6), curr block transition
        // probabilities are calculated and put into the matrix
        const double rollProb = 1.0/6.0;

        for (int dice = 1; dice <= 6; dice +=1) {
            int nextBlock = block + dice;
            int finalDestination;
            
            // CASE 1: overshooting the total states
            if (nextBlock > totalStates - 1) {
                finalDestination = block;
            }

            // CASE 2: winning block
            else if (nextBlock == totalStates - 1) {
                finalDestination = nextBlock;
            }

            else {
                auto [row, col] = nonLineariseBlock(nextBlock);
                // CASE 3: Snake / Ladder
                if (board[row][col] != nullptr)
                    finalDestination = board[row][col]->getTargetBlock();
                else // CASE 4: empty
                    finalDestination = nextBlock;
            }

            matrix[block][finalDestination] += rollProb;
        }
    }

    public:
    TransitionMatrix(std::vector<std::vector<BoardEntity*>> b, int s, int l)
        : board(b), totalStates(s), boardLength(l),
        // initialised with 0
        matrix(s, std::vector<double>(s, 0)) {}

    void calculateProbabilities() {
        for (int i =0; i < totalStates; i++) {
            calculateTransitionProbs(i);
        }
    }

    void exportToCSV(const std::string& filename) {
        std::ofstream file(filename);

        for (int i = 0; i<totalStates; i+=1) {
            for (int j=0; j< totalStates; j+=1) {
                file << matrix[i][j];
                if (j < totalStates - 1) 
                    file << ",";
            }
            file<<"\n";
        }
        file.close();
        
    }

    // Transient states: from where transitioning to another states is possible
    // Absorbing state: final state / from where transitioning isn't possible
    
    std::vector<std::vector<double>> getQMatrix() {
        // matrix with all the transient states only so:
        // size: (totalStates - 1) x (totalStates - 1)

        int transientStates = totalStates - 1;
        std::vector<std::vector<double>> Q(transientStates, std::vector<double>(transientStates));

        for (int i = 0; i < transientStates; i++) {
            for (int j = 0; j < transientStates; j++) {
                Q[i][j] = matrix[i][j];
            }
        }
        
        return Q;
    }

    std::vector<std::vector<double>> getRMatrix() {
        // transient states to absorbing state only
        // size: (totalStates - 1) x 1
        int transientStates = totalStates - 1;
        int absorbingState = totalStates - 1;
        std::vector<std::vector<double>> R(transientStates, std::vector<double>(1));

        for (int i=0; i< transientStates; i+=1) {
            R[i][0] = matrix[i][absorbingState];
        }

        return R;
    }

    std::vector<std::vector<double>> getIdentityMatrix(int n) {
        // identity matrix sized n x n

        std::vector<std::vector<double>> I(n, std::vector<double>(n, 0.0));
        
        for (int i = 0; i < n; i++) {
            I[i][i] = 1.0;
        }
        
        return I;
    }
};