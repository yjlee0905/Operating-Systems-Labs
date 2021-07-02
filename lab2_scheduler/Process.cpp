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
}