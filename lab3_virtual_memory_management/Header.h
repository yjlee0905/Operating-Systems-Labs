//
// Created by Yunjeon Lee on 2021/07/18.
//

#ifndef OPERATING_SYSTEMS_LABS_HEADER_H
#define OPERATING_SYSTEMS_LABS_HEADER_H

#include <vector>

using namespace std;

const int NUM_OF_PAGES = 64;
const int MAX_FRAMES = 128;

struct PTE { // 32 bit
    unsigned int present:1;
    unsigned int referenced:1;
    unsigned int modified:1;
    unsigned int writeProtected:1;
    unsigned int pagedOut:1;
    unsigned int pageFrameNumber:7; // maximum number of frames is 128
    // TODO free 20 bit
    unsigned int fileMapped:1;
    unsigned int hole:1;
};

typedef struct {
    PTE PTEtable[NUM_OF_PAGES]; // must contain exactly 64 page table entries(PTE)
} pte_t;

struct Frame {
    // TODO virtual mappings to the process
    int frameNum;
    int pid;
    int vpage;
    bool isFree = true;
    bool isVictim = false;
};

typedef struct {
    Frame frameTable[MAX_FRAMES];
} frame_t;

struct VMA {
    unsigned int startingVirtualPage:7; // TODO check 128 bit?
    unsigned int endingVirtualPage:7;
    unsigned int writeProtected:1;
    unsigned int fileMapped:1;
};

struct Instruction {
    char operation;
    int id; // r,w : id, c, e: procid
};

#endif //OPERATING_SYSTEMS_LABS_HEADER_H
