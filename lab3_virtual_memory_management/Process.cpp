//
// Created by Yunjeon Lee on 2021/07/18.
//

#include "Process.h"

Process::Process(int pid, vector<VMA> VMAs){
    this->pid = pid;
    this->VMAs = VMAs;
}

int Process::getPID() {return this->pid;}
vector<VMA> Process::getVMAs() {return this->VMAs;}

