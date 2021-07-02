//
// Created by Yunjeon Lee on 2021/07/01.
//

#ifndef OPERATING_SYSTEMS_LABS_PROCESS_H
#define OPERATING_SYSTEMS_LABS_PROCESS_H

#include "Constants.h"

class Process { // TODO state??
private:
    int pid;
    int arrivalTime; // AT
    int totalCPUtime; // TC
    int CPUburst; // CB
    int IOburst; // IO
public:
    process_state_t processState;
    int finishingTime; // FT
    int turnAroundTime; // TT
    int IOtime; // IT
    int priority; // PRIO
    int CPUwaitingTime; // CW

    int remainingTime; // rem
    // TODO check process creation situations
    Process(int pid, int arrivalTime, int totalCPUtime, int CPUburst, int IOburst);
};

#endif //OPERATING_SYSTEMS_LABS_PROCESS_H