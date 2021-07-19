//
// Created by Yunjeon Lee on 2021/07/18.
//

#include "Process.h"

Process::Process(int pid, vector<VMA> VMAs){
    this->pid = pid;
    this->VMAs = VMAs;

    this->maps = 0;
    this->unmaps = 0;
    this->ins = 0;
    this->outs = 0;
    this->fins = 0;
    this->fouts = 0;
    this->zeros = 0;
    this->segv = 0;
    this->segprot = 0;

    // TODO change PTEtable initialization
    for (int i = 0; i < MAX_FRAMES; i++) {
        this->pageTable.PTEtable[i].present = 0;
        this->pageTable.PTEtable[i].referenced = 0;
        this->pageTable.PTEtable[i].modified = 0;
        this->pageTable.PTEtable[i].writeProtected = 0;
        this->pageTable.PTEtable[i].pagedOut = 0;
        this->pageTable.PTEtable[i].pageFrameNumber = i;
        this->pageTable.PTEtable[i].fileMapped = 0;
    }
}

int Process::getPID() {return this->pid;}

vector<VMA> Process::getVMAs() {return this->VMAs;}

