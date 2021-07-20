//
// Created by Yunjeon Lee on 2021/07/19.
//

#ifndef OPERATING_SYSTEMS_LABS_PAGER_H
#define OPERATING_SYSTEMS_LABS_PAGER_H

#include "Header.h"

class Pager {
private:
    int hand;
    int size;
public:
    Pager(int size);
    virtual Frame* selectVictimFrame(frame_t& frameTable) = 0;
};

class FIFOpager : public Pager {
private:
    int hand;
    int size;
public:
    FIFOpager(int size);
    Frame* selectVictimFrame(frame_t& frameTable);
};

class RandomPager : public Pager {
private:
    int hand;
    int size;
    vector<int> randomNums;

    int getRandom();
public:
    RandomPager(int size, vector<int> randomNums);
    Frame* selectVictimFrame(frame_t& frameTable);
};

#endif //OPERATING_SYSTEMS_LABS_PAGER_H