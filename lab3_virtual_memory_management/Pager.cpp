//
// Created by Yunjeon Lee on 2021/07/19.
//

#include <iostream>
#include "Pager.h"
#include "Header.h"

using namespace std;

Pager::Pager(int size){}

void Pager::incrementTimer() {}

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
    this->cnt = 0; // hand
    this->size = size;
    this->head = &frameTable.frameTable[0]; // curFrame
}

Frame* ClockPager::selectVictimFrame(frame_t &frameTable, vector<Process*>& procs) {
    bool isSearchingVictim = false;

    Frame* selectedVictim;
    PTE* curPage = &procs.at(head->pid)->pageTable.PTEtable[head->vpage];

    while (!isSearchingVictim) {
        if (curPage->referenced == 1) {
            //cout << "check[" << cnt << "] " << head->pid << ":" << head->vpage << ":" << curPage->referenced << "   ";
            curPage->referenced = 0;
            cnt++;
            if (cnt == size) { cnt = 0; }
            head = &frameTable.frameTable[cnt];
            curPage = &procs.at(head->pid)->pageTable.PTEtable[head->vpage];

        } else if (curPage->referenced == 0) {
            //cout << "!!!!! check[" << cnt << "] " << head->pid << ":" << head->vpage << ":" << curPage->referenced << "   ";
            isSearchingVictim = true;
            selectedVictim = &frameTable.frameTable[curPage->pageFrameNumber];
            selectedVictim->isVictim = true;
            cnt++;
            if (cnt == size) { cnt = 0; }
            head = &frameTable.frameTable[cnt];
            return selectedVictim;
        }
    }
}

NRUpager::NRUpager(int size) : Pager(size) {
    this->hand = 0;
    this->size = size;
    this->timer = 0;
}

void NRUpager::incrementTimer() {
    this->timer++;
}

Frame* NRUpager::selectVictimFrame(frame_t &frameTable, vector<Process*>& procs) {
    Frame* selectedVictim;
    int classes[4] = {-1, -1, -1, -1};
    int fillClasses = 0;

    for (int i = 0; i < size; i++) {
        Frame* curFrame = &frameTable.frameTable[hand];
        PTE* curPage = &procs.at(curFrame->pid)->pageTable.PTEtable[curFrame->vpage];
        int classId = curPage->referenced * 2 + curPage->modified * 1;

        if (classes[classId] == -1) {
            classes[classId] = curFrame->frameNum;
            //if (fillClasses++ == 4) break; TODO check hand
        }

        hand++;
        if (hand == size) {
            //cout << "set to 0" << endl;
            hand = 0;
        }
    }

    for (int i = 0; i < 4; i++) {
        if (classes[i] != -1) {
            selectedVictim = &frameTable.frameTable[classes[i]];
            selectedVictim->isVictim = true;
            hand = selectedVictim->frameNum + 1;
            if (hand == size) {
                hand = 0;
            }
            break;
        }
    }

    if (timer >= TIME_LIMIT) {
        for (int i = 0; i < size; i++) {
            if (frameTable.frameTable[i].pid != -1) {
                procs.at(frameTable.frameTable[i].pid)->pageTable.PTEtable[frameTable.frameTable[i].vpage].referenced = 0;
            }
        }
        timer = 0;
    }
    return selectedVictim;
}

AgingPager::AgingPager(int size) : Pager(size) {
    this->hand = 0;
    this->size = size;
}

Frame* AgingPager::selectVictimFrame(frame_t &frameTable, vector<Process *> &procs) {
    Frame* selectedVictim = &frameTable.frameTable[hand];

    for (int i = 0; i < size; i++) {
        Frame* curFrame = &frameTable.frameTable[hand];
        curFrame->age = curFrame->age >> 1;
        if (procs.at(curFrame->pid)->pageTable.PTEtable[curFrame->vpage].referenced == 1) {
            curFrame->age = (curFrame->age | 0x80000000);
            procs[curFrame->pid]->pageTable.PTEtable[curFrame->vpage].referenced = 0;
        }

        if (curFrame->age < selectedVictim->age) {
            selectedVictim = curFrame;
        }
        hand++;
        if (hand == size) {
            hand = 0;
        }
    }
    selectedVictim->isVictim = true;
    return selectedVictim;
}


