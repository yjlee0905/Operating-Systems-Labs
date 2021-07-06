//
// Created by Yunjeon Lee on 2021/07/03.
//

#ifndef OPERATING_SYSTEMS_LABS_SCHEDULER_H
#define OPERATING_SYSTEMS_LABS_SCHEDULER_H

#include <deque>
#include "Process.h"

using namespace std;

class Scheduler {
private:
    int quantum;
public:
    deque<Process*> runQ;

    Scheduler(int quantum);
    virtual void addProcess(Process* p);
    virtual Process* getNextProcess();
    // virtual void testPreempt(Process* p, int curTime); TODO
    // access private data member
    virtual int getQuantum();
    // for debug
    virtual int getProcessCount();
    virtual void showSchedulerStatus();
};

class FCFSsched : public Scheduler {
private:
    int quantum;
public:
    deque<Process*> runQ;

    FCFSsched(int quantum);
    void addProcess(Process* p);
    Process* getNextProcess();
    // access private data member
    int getQuantum();
    // for debug
    int getProcessCount();
    void showSchedulerStatus();
};

class LCFSsched : public Scheduler {
private:
    int quantum;
public:
    deque<Process*> runQ;

    LCFSsched(int quantum);
    void addProcess(Process* p);
    Process* getNextProcess();
    // access private data member
    int getQuantum();
    // for debug
    int getProcessCount();
    void showSchedulerStatus();
};

class SRTFsched : public Scheduler {
private:
    int quantum;
public:
    deque<Process*> runQ;

    SRTFsched(int quantum);
    void addProcess(Process* p);
    Process* getNextProcess();
    // access private data member
    int getQuantum();
    // for debug
    int getProcessCount();
    void showSchedulerStatus();

};

class RRsched : public Scheduler {
private:
    int quantum;
public:
    deque<Process*> runQ;

    RRsched(int quantum);
    void addProcess(Process* p);
    Process* getNextProcess();
    // access private data member
    int getQuantum();
    // for debug
    int getProcessCount();
    void showSchedulerStatus();
};
#endif //OPERATING_SYSTEMS_LABS_SCHEDULER_H