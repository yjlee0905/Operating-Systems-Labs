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

Frame* FIFOpager::selectVictimFrame(frame_t& frameTable, vector<Process*>& procs) {
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

Frame* RandomPager::selectVictimFrame(frame_t &frameTable, vector<Process*>& procs) {
    int idx = getRandom(); // TODO size
    frameTable.frameTable[idx].isVictim = true;
    return &frameTable.frameTable[idx];
}

ClockPager::ClockPager(int size, frame_t& frameTable) : Pager(size) {
    //this->hand = 0;
    this->cnt = 0;
    this->size = size;
    this->head = &frameTable.frameTable[0];
}

Frame* ClockPager::selectVictimFrame(frame_t &frameTable, vector<Process*>& procs) {
    bool isFinish = false;

    Frame* selectedVictim;
    PTE* correspondingPage = &procs.at(head->pid)->pageTable.PTEtable[head->vpage];

    while (!isFinish) {
        if (correspondingPage->referenced == 1) {
            //cout << "check[" << cnt << "] " << head->pid << ":" << head->vpage << ":" << correspondingPage->referenced << "   ";
            correspondingPage->referenced = 0;
            cnt++;
            if (cnt == size) { cnt = 0; }
            head = &frameTable.frameTable[cnt];
            correspondingPage = &procs.at(head->pid)->pageTable.PTEtable[head->vpage];

        } else if (correspondingPage->referenced == 0) {
            //cout << "!!!!! check[" << cnt << "] " << head->pid << ":" << head->vpage << ":" << correspondingPage->referenced << "   ";
            isFinish = true;
            selectedVictim = &frameTable.frameTable[correspondingPage->pageFrameNumber];
            selectedVictim->isVictim = true;
            cnt++;
            if (cnt == size) { cnt = 0; }
            head = &frameTable.frameTable[cnt];
            return selectedVictim;
        } else {
            cout << "invalid" << endl;
        }
    }
}


