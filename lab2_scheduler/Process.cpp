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

    this->finishingTime = 0;
    this->IOtime = 0;
    this->CPUwaitingTime = 0;

    this->processState = STATE_CREATED;
    this->staticPriority = priority;
    this->dynamicPriority = priority-1;

    this->curCPUburst = 0;
    this->curIOburst = 0;
    this->curRemainingTime = totalCPUtime;

    this->timeInPrevState = 0;
    this->stateTs = arrivalTime;
}

int Process::getPID() {return this->pid;}
int Process::getArrivalTime() {return this->arrivalTime;}
int Process::getTotalCPUtime() {return this->totalCPUtime;}
int Process::getCPUburst() {return this->CPUburst;}
int Process::getIOburst() {return this->IOburst;}