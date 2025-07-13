#pragma once
#include "boardEntity.hpp"

class Snake: public BoardEntity {
    const int start;
    const int end;
    
    public:
    Snake(const int s, const int e): start(s), end(e) {}
    int getStart() const override { return start; }
    int getEnd() const override { return end; }
};