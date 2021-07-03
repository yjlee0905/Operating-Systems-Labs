//
// Created by Yunjeon Lee on 2021/07/03.
//

#ifndef OPERATING_SYSTEMS_LABS_SCHEDULER_H
#define OPERATING_SYSTEMS_LABS_SCHEDULER_H

#include <deque>
#include "Process.h"

using namespace std;

class Scheduler{
public:
    Scheduler();
    virtual void addProcess(Process* p);
    virtual Process* getNextProcess();
    // virtual void testPreempt(Process* p, int curTime); TODO
};

class FCFS : public Scheduler{
private:
    deque<Process*> readyQ;
public:
    FCFS();
    void addProcess(Process* p);
    Process* getNextProcess();
};

#endif //OPERATING_SYSTEMS_LABS_SCHEDULER_H