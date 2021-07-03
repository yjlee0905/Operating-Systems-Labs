//
// Created by Yunjeon Lee on 2021/07/01.
//

#include "Process.h"

Process::Process(int pid, int arrivalTime, int totalCPUtime, int CPUburst, int IOburst) {
    this->pid = pid;
    this->arrivalTime = arrivalTime;
    this->totalCPUtime = totalCPUtime;
    this->CPUburst = CPUburst;
    this->IOburst = IOburst;

    this->curRemainingTime = arrivalTime;
    this->prevStateDuration = 0;
}

int Process::getPID() {return this->pid;}
int Process::getTotalCPUburst() {return this->CPUburst;}
int Process::getTotalIOburst() {return this->IOburst;}