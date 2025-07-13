#pragma once

class BoardEntity {
    public:
    virtual int getStart() const = 0;
    virtual int getEnd() const = 0;
    virtual ~BoardEntity() {}
};