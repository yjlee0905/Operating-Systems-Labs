//
// Created by Yunjeon Lee on 2021/07/19.
//

#ifndef OPERATING_SYSTEMS_LABS_PAGER_H
#define OPERATING_SYSTEMS_LABS_PAGER_H

#include "Header.h"
#include "Process.h"

class Pager {
private:
    int hand;
    int size;
public:
    Pager(int size);
    virtual Frame* selectVictimFrame(frame_t& frameTable, vector<Process*>& procs) = 0;
};

class FIFOpager : public Pager {
private:
    int hand;
    int size;
public:
    FIFOpager(int size);
    Frame* selectVictimFrame(frame_t& frameTable, vector<Process*>& procs);
};

class RandomPager : public Pager {
private:
    int hand;
    int size;
    vector<int> randomNums;

    int getRandom();
public:
    RandomPager(int size, vector<int> randomNums);
    Frame* selectVictimFrame(frame_t& frameTable, vector<Process*>& procs);
};

class ClockPager : public Pager {
private:
    //int hand;
    int cnt;
    int size;
    Frame* head;
public:
    ClockPager(int size, frame_t& frameTable);
    Frame* selectVictimFrame(frame_t& frameTable, vector<Process*>& procs);
};

#endif //OPERATING_SYSTEMS_LABS_PAGER_H