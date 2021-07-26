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

    for (int i = 0; i < size; i++) {
        Frame* curFrame = &frameTable.frameTable[hand];
        PTE* curPage = &procs.at(curFrame->pid)->pageTable.PTEtable[curFrame->vpage];
        int classId = curPage->referenced * 2 + curPage->modified * 1;

        if (classes[classId] == -1) {
            classes[classId] = curFrame->frameNum;
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

    hand = selectedVictim->frameNum + 1;
    if (hand == size) {
        hand = 0;
    }
    selectedVictim->isVictim = true;
    return selectedVictim;
}

WorkingSetPager::WorkingSetPager(int size) : Pager(size) {
    this->hand = 0;
    this->size = size;
    this->timer = 0;
}

void WorkingSetPager::incrementTimer() {
    this->timer++;
}

Frame* WorkingSetPager::selectVictimFrame(frame_t &frameTable, vector<Process *> &procs) {
    Frame* selectedVictim;
    int start = hand;
    //int tmp = start; for debugging
    for (int i = 0; i < size; i++) {
        Frame* curFrame = &frameTable.frameTable[hand];
        PTE* curPage = &procs.at(curFrame->pid)->pageTable.PTEtable[curFrame->vpage];
        if (curPage->referenced == 1) {
            curPage->referenced = 0;
            curFrame->timeOfLastUse = timer;
        } else {
            if (((timer - curFrame->timeOfLastUse) > TAU)) {
                selectedVictim = curFrame;
                hand = selectedVictim->frameNum + 1;
                if (hand == size) {hand = 0;}
                selectedVictim = curFrame;
                selectedVictim->isVictim = true;
                return selectedVictim;
            }

        }
        hand++;
        if (hand == size) {hand = 0;}
    }

    selectedVictim = &frameTable.frameTable[start];
    for (int i = 0; i < size; i++) {
        Frame* curFrame = &frameTable.frameTable[start];

        if (selectedVictim->timeOfLastUse > curFrame->timeOfLastUse) {
            selectedVictim = curFrame;
        }
        start++;
        if (start == size) {start = 0;}
    }

    //ASELECT 0-15 | 0(0 0:9 25) 1(0 0:59 26) 2(0 0:54 27) 3(0 0:2 28) 4(0 0:62 29) 5(0 0:16 30) 6(0 0:47 31) 7(0 0:58 32) 8(0 0:1 33) 9(0 0:30 34) 10(0 0:35 35) 11(0 0:55 36) 12(0 0:57 37) 13(0 0:20 38) 14(0 0:17 39) 15(1 0:28 39) | 0
//    cout << "ASELECT " << tmp << "- | ";
//    for (int i = 0; i < size; i++) {
//        Frame* curFrame = &frameTable.frameTable[tmp];
//        PTE* curPage = &procs.at(curFrame->pid)->pageTable.PTEtable[curFrame->vpage];
//        cout << tmp << "(" << curPage->referenced << " " << curFrame->pid << ":" << curFrame->vpage << " " << curFrame->timeOfLastUse << ") ";
//
//        tmp++;
//        if (tmp == size) { tmp = 0;}
//    }
//    cout << endl;

    hand = selectedVictim->frameNum + 1;
    if (hand == size) {hand = 0;}
    selectedVictim->isVictim = true;
    return selectedVictim;
}