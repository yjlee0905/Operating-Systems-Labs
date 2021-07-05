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
    // for debug
    virtual int getProcessCount();
    virtual void showShedulerStatus();
};

class FCFSsched : public Scheduler{
private:
    deque<Process*> runQ;
public:
    FCFSsched();
    void addProcess(Process* p);
    Process* getNextProcess();
    // for debug
    int getProcessCount();
    void showShedulerStatus();
};

#endif //OPERATING_SYSTEMS_LABS_SCHEDULER_H