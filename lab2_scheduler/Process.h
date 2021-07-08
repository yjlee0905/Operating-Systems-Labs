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
    bool isExpired;
    process_state_t prevState;
    int finishingTime; // FT
    int turnAroundTime; // TT
    int IOtime; // IT
    int staticPriority; // PRIO
    int dynamicPriority;
    int CPUwaitingTime; // CW

    // current info
    int curCPUburst; // cb
    int curIOburst; // ib
    int curRemainingTime; // rem

    // prev info
    int timeInPrevState; // how long the process was in the prev state
    int stateTs;

    Process(int pid, int arrivalTime, int totalCPUtime, int CPUburst, int IOburst, int priority);
    int getPID();
    int getArrivalTime();
    int getTotalCPUtime();
    int getCPUburst();
    int getIOburst();
};

#endif //OPERATING_SYSTEMS_LABS_PROCESS_H