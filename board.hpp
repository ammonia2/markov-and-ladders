#pragma once
#include <iostream>
#include <vector>
#include <random>
#include "boardEntity.hpp"
#include "snake.hpp"
#include "ladder.hpp"

class Board {
    std::vector<std::vector<BoardEntity*>> board;
    const int snakesCount, ladderCount, boardLength, boardHeight;
    
    double getSnakeProbability(const int currBlock, const int totalBlocks) {
        // lower probability of placement at the start & higher at the end
        double p = static_cast<double>(currBlock) / totalBlocks;
        return p;
    }

    double getLadderProbability(const int currBlock, const int totalBlocks) {
        // higher probability of placement at the start & vice versa
        double p = static_cast<double>(currBlock) / totalBlocks;
        p = 1 - p;
        return p;
    }

    void initialiseBoard(
        int snakesCount, int ladderCount,
        const int boardLength, const int boardHeight
    ) {
        
        // not all snakes & ladders are placed to prevent board from becoming overcrowded
        const double placementThreshold = 0.3;
        const int totalBlocks = boardLength * boardHeight;
        std::vector<bool> occupiedPositions(totalBlocks, false);

        // board init rules:
        // 1. more ladders in early blocks (40% of the board)
        // 2. more snakes in later blocks (>= 60%)
        // 3. no snakes / ladders in adjacent blocks
        // 4. longest snake no more than 90% of total blocks
        // 5. no snakes / ladders at the starting block

        // random number generator and distribution
        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::uniform_real_distribution<> dis(0.0, 1.0);

        for (int i = 0; i < boardHeight; i++) {
            for (int j = 0; j < boardLength; j++) {
                const int currentLinearBlock = i * boardLength + j;

                // no snake/ladder at the starting (0) or ending block (totalBlocks - 1)
                if (currentLinearBlock == 0 || currentLinearBlock == totalBlocks - 1) {
                    continue;
                }
                
                // if this position is already occupied (start or end of another snake/ladder)
                if (occupiedPositions[currentLinearBlock])
                    continue;
                
                // checking horizontal adjacency (simplified for now)
                bool adjacentOccupied = false;
                if (j > 0 && board[i][j-1] != nullptr) adjacentOccupied = true;
                if (j < boardLength - 1 && board[i][j+1] != nullptr) adjacentOccupied = true;
                if (adjacentOccupied) {
                    continue;
                }

                const double snakeP = getSnakeProbability(currentLinearBlock, totalBlocks);
                const double ladderP = getLadderProbability(currentLinearBlock, totalBlocks);
                
                // random number for selection
                double selectionRoll = dis(gen);
                
                
                // comparing relative probabilities
                double totalP = snakeP + ladderP;
                double normalizedLadderP = ladderP / totalP;
                
                if ((ladderCount > 0 || snakesCount > 0) && selectionRoll < totalP * placementThreshold) {
                    
                    if (ladderCount > 0 && selectionRoll < totalP * placementThreshold * normalizedLadderP) { // ladder placement
                        if (currentLinearBlock < totalBlocks - 1) { // cannot place ladder if there's no space above
                            const int minPossibleEnd = currentLinearBlock + 1;
                            const int maxPossibleEnd = totalBlocks - 1;
                            if (minPossibleEnd <= maxPossibleEnd) {
                                std::uniform_int_distribution<> distLadderEnd(minPossibleEnd, maxPossibleEnd);
                                const int endBlock = distLadderEnd(gen);
                                
                                if (!occupiedPositions[endBlock]) { // if ending pos not occupied
                                    board[i][j] = new Ladder(currentLinearBlock, endBlock);
                                    occupiedPositions[currentLinearBlock] = true;
                                    occupiedPositions[endBlock] = true;
                                    ladderCount--;
                                }
                            }
                        }
                    }

                    else if (snakesCount > 0) {
                        if (currentLinearBlock > 0) { // cannot place snake if no space below
                            const int minPossibleEnd = 0;
                            const int maxPossibleEnd = currentLinearBlock - 1;
                            if (minPossibleEnd <= maxPossibleEnd) {
                                std::uniform_int_distribution<> distSnakeEnd(minPossibleEnd, maxPossibleEnd);
                                const int endBlock = distSnakeEnd(gen);
                                
                                // longest snake no more than 90% of total blocks
                                if (currentLinearBlock - endBlock <= 0.9 * totalBlocks) {
                                    if (!occupiedPositions[endBlock]) {
                                        board[i][j] = new Snake(currentLinearBlock, endBlock);
                                        occupiedPositions[currentLinearBlock] = true;
                                        occupiedPositions[endBlock] = true;
                                        snakesCount--;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    public:
    Board(const int snakes, const int ladders, int length, int height)
        : snakesCount(snakes), ladderCount(ladders), boardLength(length), boardHeight(height),
        board(height, std::vector<BoardEntity*>(length, nullptr)) {
        
        initialiseBoard(snakes, ladders, length, height);
    }

    void displayBoard() {
        for (size_t i = 0; i < board.size(); ++i) {
            for (size_t j = 0; j < board[i].size(); ++j) {
                if (board[i][j] == nullptr) {
                    std::cout << ". ";
                } else if (dynamic_cast<Snake*>(board[i][j])) {
                    std::cout << "S ";
                } else if (dynamic_cast<Ladder*>(board[i][j])) {
                    std::cout << "L ";
                } else {
                    std::cout << "? ";
                }
            }
            std::cout << std::endl;
        }
    }
    
    std::vector<std::vector<BoardEntity*>> getBoard() {
        return board;
    }
};