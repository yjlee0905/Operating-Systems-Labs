//
// Created by Yunjeon Lee on 2021/07/19.
//

#include <iostream>
#include "Pager.h"

using namespace std;

Pager::Pager(int size){}

FIFOpager::FIFOpager(int size) : Pager(size) {
    this->hand = 0;
    this->size = size;
}

Frame* FIFOpager::selectVictimFrame(frame_t& frameTable) {
    Frame* selectedVictim = &frameTable.frameTable[hand];
    hand++;
    if (hand == this->size) {
        hand = 0;
    }
    selectedVictim->isVictim = true;
    return selectedVictim;
}

RandomPager::RandomPager(int size, vector<int> randomNums) : Pager(size) {
    this->hand = 0;
    this->size = size;
    this->randomNums = randomNums;
}

int RandomPager::getRandom() {
    int num = randomNums[hand++] % size;
    if (hand == randomNums.size()) { // TODO check
        hand = 0;
    }
    return num;
}

Frame* RandomPager::selectVictimFrame(frame_t &frameTable) {
    int idx = getRandom(); // TODO size
    frameTable.frameTable[idx].isVictim = true;
    return &frameTable.frameTable[idx];
}


