//
// Created by Yunjeon Lee on 2021/07/01.
//

#ifndef OPERATING_SYSTEMS_LABS_PROCESS_H
#define OPERATING_SYSTEMS_LABS_PROCESS_H

class Process {
private:
    int pid;
    int arrivalTime;
    int totalCPUtime;
    int CPUburst;
    int IOburst;
public:
    // TODO check process creation situations
    Process(int pid, int arrivalTime, int totalCPUtime, int CPUburst, int IOburst);
};

#endif //OPERATING_SYSTEMS_LABS_PROCESS_H