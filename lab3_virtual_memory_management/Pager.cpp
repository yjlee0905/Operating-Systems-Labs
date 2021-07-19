//
// Created by Yunjeon Lee on 2021/07/19.
//

#include <iostream>
#include "Pager.h"

using namespace std;

Pager::Pager(int size){}

FIFOpager::FIFOpager(int size) : Pager(size) {
    this->hand = 0;
}

Frame* FIFOpager::selectVictimFrame(frame_t& frameTable) {
    Frame* selectedVictim = &frameTable.frameTable[hand];
    hand++;
    if (hand == size) {
        hand = 0;
    }
    selectedVictim->isVictim = true;
    return selectedVictim;
}


