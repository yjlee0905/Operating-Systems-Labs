//
// Created by Yunjeon Lee on 2021/07/18.
//

#ifndef OPERATING_SYSTEMS_LABS_HEADER_H
#define OPERATING_SYSTEMS_LABS_HEADER_H

#include <vector>

using namespace std;

const int NUM_OF_PAGES = 64;
const int MAX_FRAMES = 128;

const unsigned long long COST_RW_INSTR = 1;
const unsigned long long COST_CTX_SWITCHES = 130;
const unsigned long long COST_PROC_EXITS = 1250;
const unsigned long long COST_MAPS = 300;
const unsigned long long COST_UNMAPS = 400;
const unsigned long long COST_INS = 3100;
const unsigned long long COST_OUTS = 2700;
const unsigned long long COST_FINS = 2800;
const unsigned long long COST_FOUTS = 2400;
const unsigned long long COST_ZEROS = 140;
const unsigned long long COST_SEGV = 340;
const unsigned long long COST_SEGPROT = 420;

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
    unsigned long long age;
    unsigned long long timeOfLastUse;
};

typedef struct {
    Frame frameTable[MAX_FRAMES];
} frame_t;

struct VMA {
    int startingVirtualPage;
    int endingVirtualPage;
    unsigned int writeProtected:1;
    unsigned int fileMapped:1;
};

struct Instruction {
    char operation;
    int id; // r,w : id, c,e: procid
};

#endif //OPERATING_SYSTEMS_LABS_HEADER_H
