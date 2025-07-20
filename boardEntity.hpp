#pragma once

class BoardEntity {
    // const int position;

    public:
    virtual int getStart() const = 0;
    virtual int getEnd() const = 0;
    virtual int getTargetBlock() {
        return NULL;
    }
    virtual ~BoardEntity() {}
};