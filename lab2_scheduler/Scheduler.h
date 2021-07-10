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
    // TODO change to private
    deque<Process*> runQ;
    deque<Process*> *activeQ;
    deque<Process*> *expiredQ;

    Scheduler(int quantum);
    virtual void addProcess(Process* p) = 0;
    virtual Process* getNextProcess() = 0;
    // virtual void testPreempt(Process* p, int curTime); TODO
    // access private data member
    virtual int getQuantum() = 0;
    // for debug, TODO remove
    virtual bool shouldPreempt(Process* curProc, Process* proc, int t, int currentTime);
    virtual int getProcessCount() = 0;
    virtual void showSchedulerStatus() = 0;
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

class PRIOsched : public Scheduler {
protected:
    int quantum;
    int maxPrios;

    bool isMLQempty(deque<Process*> *q);
public:
    deque<Process*> *activeQ;
    deque<Process*> *expiredQ;
    deque<Process*> q1[32];
    deque<Process*> q2[32];


    //PRIOsched(int quantum);
    PRIOsched(int quantum, int maxPrios);

    void addProcess(Process* p);
    Process* getNextProcess();
    // access private data member
    int getQuantum();
    // for debug
    int getProcessCount();
    void showSchedulerStatus();
};

class PREPRIOsched : public PRIOsched {
public:
    PREPRIOsched(int quantum, int maxPrios);
    bool shouldPreempt(Process* curProc, Process* proc, int t, int currentTime);

};
#endif //OPERATING_SYSTEMS_LABS_SCHEDULER_H