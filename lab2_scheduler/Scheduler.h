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
    Scheduler(int quantum);
    virtual void addProcess(Process* p) = 0;
    virtual Process* getNextProcess() = 0;
    virtual bool shouldPreempt(Process* curRunningProc, Process* proc, int evtTime, int currentTime);
    // to print header in the statistics efficiently, not used in real simulation
    virtual void printAlgoInfo() = 0;
    // access private data member
    virtual int getQuantum() = 0;
    // for debug
    virtual int getProcessCount() = 0;
    virtual void showSchedulerStatus() = 0;
};

class FCFSsched : public Scheduler {
private:
    deque<Process*> runQ;
    int quantum;
public:

    FCFSsched(int quantum);
    void addProcess(Process* p);
    Process* getNextProcess();
    // to print header in the statistics efficiently, not used in real simulation
    void printAlgoInfo();
    // access private data member
    int getQuantum();
    // for debug
    int getProcessCount();
    void showSchedulerStatus();
};

class LCFSsched : public Scheduler {
private:
    deque<Process*> runQ;
    int quantum;
public:

    LCFSsched(int quantum);
    void addProcess(Process* p);
    Process* getNextProcess();
    // to print header in the statistics efficiently, not used in real simulation
    void printAlgoInfo();
    // access private data member
    int getQuantum();
    // for debug
    int getProcessCount();
    void showSchedulerStatus();
};

class SRTFsched : public Scheduler {
private:
    deque<Process*> runQ;
    int quantum;
public:

    SRTFsched(int quantum);
    void addProcess(Process* p);
    Process* getNextProcess();
    // to print header in the statistics efficiently, not used in real simulation
    void printAlgoInfo();
    // access private data member
    int getQuantum();
    // for debug
    int getProcessCount();
    void showSchedulerStatus();
};

class RRsched : public Scheduler {
private:
    deque<Process*> runQ;
    int quantum;
public:

    RRsched(int quantum);
    void addProcess(Process* p);
    Process* getNextProcess();
    // to print header in the statistics efficiently, not used in real simulation
    void printAlgoInfo();
    // access private data member
    int getQuantum();
    // for debug
    int getProcessCount();
    void showSchedulerStatus();
};

class PRIOsched : public Scheduler {
protected:
    deque<Process*> *activeQ;
    deque<Process*> *expiredQ;
    deque<Process*> q1[32];
    deque<Process*> q2[32];
    /*
     * deque<Process*> *activeQ;
     * deque<Process*> *expiredQ;
     *
     * And initialize in constructor as below.
     *
     * this->activeQ = new deque<Process*>[maxPrios];
     * this->expiredQ = new deque<Process*>[maxPrios];
     *
     * This does not works, so set to 32
     * */

    int quantum;
    int maxPrios;

public:

    PRIOsched(int quantum, int maxPrios);
    void addProcess(Process* p);
    Process* getNextProcess();
    // to print header in the statistics efficiently, not used in real simulation
    void printAlgoInfo();
    // access private data member
    int getQuantum();
    // for debug
    int getProcessCount();
    void showSchedulerStatus();
};

class PREPRIOsched : public PRIOsched {
public:
    PREPRIOsched(int quantum, int maxPrios);
    bool shouldPreempt(Process* curRunningProc, Process* proc, int evtTime, int currentTime);
    // to print header in the statistics efficiently, not used in real simulation
    void printAlgoInfo();
};
#endif //OPERATING_SYSTEMS_LABS_SCHEDULER_H