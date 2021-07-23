//
// Created by Yunjeon Lee on 2021/07/18.
//

#ifndef OPERATING_SYSTEMS_LABS_HEADER_H
#define OPERATING_SYSTEMS_LABS_HEADER_H

#include <vector>

using namespace std;

const int NUM_OF_PAGES = 64;
const int MAX_FRAMES = 128;

const int COST_RW_INSTR = 1;
const int COST_CTX_SWITCHES = 130;
const int COST_PROC_EXITS = 1250;
const int COST_MAPS = 300;
const int COST_UNMAPS = 400;
const int COST_INS = 3100;
const int COST_OUTS = 2700;
const int COST_FINS = 2800;
const int COST_FOUTS = 2400;
const int COST_ZEROS = 140;
const int COST_SEGV = 340;
const int COST_SEGPROT = 420;

const int TIME_LIMIT = 50;

const int TAU = 49;

struct PTE { // 32 bit
    unsigned int present:1;
    unsigned int referenced:1;
    unsigned int modified:1;
    unsigned int writeProtected:1;
    unsigned int pagedOut:1;
    unsigned int pageFrameNumber:7; // maximum number of frames is 128
    // TODO free 20 bit
    unsigned int fileMapped:1;
    unsigned int notHole:1;
    unsigned int checkedHole:1;
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
    unsigned long age;
    unsigned long timeOfLastUse;
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
