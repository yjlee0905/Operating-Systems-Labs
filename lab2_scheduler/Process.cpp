//
// Created by Yunjeon Lee on 2021/07/01.
//

#include "Process.h"

Process::Process(int pid, int arrivalTime, int totalCPUtime, int CPUburst, int IOburst, int priority) {
    this->pid = pid;
    this->arrivalTime = arrivalTime;
    this->totalCPUtime = totalCPUtime;
    this->CPUburst = CPUburst;
    this->IOburst = IOburst;

    this->processState = STATE_CREATED;
    this->staticPriority = priority;
    this->dynamicPriority = priority-1; // TODO check

    this->curCPUburst = 0;
    this->curIOburst = 0;
    this->curRemainingTime = totalCPUtime;

    this->timeInPrevState = 0;
    this->stateTs = arrivalTime;
}

int Process::getPID() {return this->pid;}
int Process::getTotalCPUburst() {return this->CPUburst;}
int Process::getTotalIOburst() {return this->IOburst;}