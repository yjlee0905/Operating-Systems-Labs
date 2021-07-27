//
// Created by Yunjeon Lee on 2021/07/18.
//

#ifndef OPERATING_SYSTEMS_LABS_PROCESS_H
#define OPERATING_SYSTEMS_LABS_PROCESS_H

#include <vector>
#include "Header.h"

using namespace std;

class Process {
private:
    int pid;
    vector<VMA> VMAs;

public:
    pte_t pageTable; // represents the translations from virtual pages to physical frames for that process

    // for statistics
    unsigned long maps;
    unsigned long unmaps;
    unsigned long ins;
    unsigned long outs;
    unsigned long fins;
    unsigned long fouts;
    unsigned long zeros;
    unsigned long segv;
    unsigned long segprot;

    Process(int pid, vector<VMA> VMAs);
    // access private data member
    int getPID();
    vector<VMA> getVMAs();
};

#endif //OPERATING_SYSTEMS_LABS_PROCESS_H
