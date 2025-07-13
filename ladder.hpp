#pragma once
#include "boardEntity.hpp"

class Ladder: public BoardEntity {
    const int start;
    const int end;
    
    public:
    Ladder(const int s, const int e): start(s), end(e) {}
    int getStart() const override { return start; }
    int getEnd() const override { return end; }
};