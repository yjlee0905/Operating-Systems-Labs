//
// Created by Yunjeon Lee on 2021/06/28.
//

#include "Event.h"
#include "Process.h"
#include "Scheduler.h"
#include <getopt.h>
#include <fstream>
#include <iostream>
#include <queue>
#include <string>
#include <string.h>
#include <vector>

using namespace std;

void initEventQueue(string fileName, int maxprio);
void readRandomNums(string fileName);
int getRandom(int burst);
void handleTransToReady(Event *evt);
void handleTransToRun(Event *evt);
void handleTransToBlock(Event *evt);
void handleTransToPreempt(Event *evt);
void handleTransToDone(Event *evt);
void printStatistics();
void printVerbose(int currentTime, Event *evt);

EventQueue *evtQueue = new EventQueue();
deque<Process *> results;
vector<int> randomNums; // max : 4611686018427387903(built as 64-bit target), 1073741823(built as 32-bit target)
int ofs = 0;

// scheduler
Scheduler* scheduler;
bool callScheduler = false;
Process *currentRunningProcess = nullptr;

// IO utilization
int timeIObusy = 0;
int startIOtime = 0;
int finishIOtime = 0;
bool isDoingIO = false;

int main(int argc, char* argv[]) {
    int c;
    char schedType;
    bool isVerbose = false;
    int quantum = MAX_QUANTUM;
    int maxprio = DEFAULT_MAX_PRIO;

    while ((c = getopt(argc,argv,"vets:")) != -1) {
        switch(c) {
            case 's':
                sscanf(optarg, "%c%d:%d", &schedType, &quantum, &maxprio);
                //schedtype = atoi(optarg);
                break;
            case 'v':
                isVerbose = true;
                break;
            case 'e':
                // not implemented
                break;
            case 't':
                // not implemented
                break;
            default:
                cout << c << " is unsupported option in this program." << endl;
                exit(1);
        }
    }

    switch (schedType)
    {
        case 'F':
            scheduler = new FCFSsched(quantum);
            break;
        case 'L':
            scheduler = new LCFSsched(quantum);
            break;
        case 'S':
            scheduler = new SRTFsched(quantum);
            break;
        case 'R':
            scheduler = new RRsched(quantum);
            break;
        case 'P':
            scheduler = new PRIOsched(quantum, maxprio);
            break;
        case 'E':
            scheduler = new PREPRIOsched(quantum, maxprio);
            break;
        default:
            cout << schedType << " is unsupported Scheduler type in this program." << endl;
            exit(1);
    }

    string inputFileName = argv[optind];
    string rFileName = argv[optind + 1];;

    readRandomNums(rFileName);
    initEventQueue(inputFileName, maxprio);

    // simulation
    Event *evt;
    while ((evt = evtQueue->getEvent())) {
        int currentTime = evt->evtTimeStamp;

        switch (evt->transition) {
            case TRANS_TO_READY:
                handleTransToReady(evt);
                if (isVerbose) printVerbose(currentTime, evt);
                break;
            case TRANS_TO_RUN:
                handleTransToRun(evt);
                if (isVerbose) printVerbose(currentTime, evt);
                break;
            case TRANS_TO_BLOCK:
                handleTransToBlock(evt);
                if (isVerbose) printVerbose(currentTime, evt);
                break;
            case TRANS_TO_PREEMPT:
                handleTransToPreempt(evt);
                if (isVerbose) printVerbose(currentTime, evt);
                break;
            case TRANS_TO_DONE:
                handleTransToDone(evt);
                if (isVerbose) printVerbose(currentTime, evt);
                break;
        }

        // remove current event object from Memory (completed event processing)
        delete evt;
        evt = nullptr;

        if (callScheduler) {
            if (evtQueue->getNextEventTime() == currentTime) {
                continue; // process next event from Event Queue
            }
            callScheduler = false;
            if (currentRunningProcess == nullptr) {
                currentRunningProcess = scheduler->getNextProcess();
                if (currentRunningProcess == nullptr)
                    continue;

                // create event to make process runnable for same time.
                Event *e = new Event(currentTime, currentRunningProcess, TRANS_TO_RUN, currentRunningProcess->processState, STATE_RUNNING);
                evtQueue->putEvent(e);
            }
        }
    }
    printStatistics();
}

void handleTransToReady(Event *evt) {
    Process *proc = evt->process; // this is the process the event works on
    int currentTime = evt->evtTimeStamp;
    int timeInPrevState = currentTime - proc->stateTs;

    // prevState: Created, Running, Blocked, Preempted
    // nextState: Running

    // All: When a process returns from I/O tis dynamic priority is reset to (staticPriority-1)
    if (proc->processState == STATE_BLOCKED) {
        proc->dynamicPriority = proc->staticPriority - 1;
    }

    proc->processState = STATE_READY;
    proc->curIOburst = 0;
    proc->timeInPrevState = timeInPrevState;
    proc->stateTs = currentTime;
    if (evt->oldState == STATE_READY) {
        proc->CPUwaitingTime += proc->timeInPrevState;
    }

    scheduler->addProcess(proc);

    int evtTime = evtQueue->getEventTimeWithProcess(currentRunningProcess);
    if ((evtTime > -1) && scheduler->shouldPreempt(currentRunningProcess, proc, evtTime, currentTime)) {
        // if both conditions are true, then create preemption evt for current running process at current time
        evtQueue->removeEvent(currentRunningProcess, currentTime, scheduler->getQuantum());
        Event* e = new Event(currentTime, currentRunningProcess, TRANS_TO_PREEMPT, currentRunningProcess->processState, STATE_READY);
        evtQueue->putEvent(e);
    }
    callScheduler = true;
}

void handleTransToRun(Event *evt) {
    Process *proc = evt->process; // this is the process the event works on
    int currentTime = evt->evtTimeStamp;
    int timeInPrevState = currentTime - proc->stateTs;

    // prevState: Ready
    // nextState: Ready, Blocked, Done
    proc->processState = STATE_RUNNING;
    if ((evt->oldState == STATE_READY) && (proc->curCPUburst == 0)) {
        proc->curCPUburst = getRandom(proc->getCPUburst()); // transition 2: Read > Running
        if (proc->curCPUburst > proc->curRemainingTime) {
            proc->curCPUburst = proc->curRemainingTime;
        }
    }

    proc->curIOburst = 0;
    proc->timeInPrevState = timeInPrevState;
    proc->stateTs = currentTime;
    if (evt->oldState == STATE_READY) {
        proc->CPUwaitingTime += proc->timeInPrevState;
    }

    int evtTimestamp;
    Event *e;
    if (scheduler->getQuantum() < proc->curCPUburst) {
        evtTimestamp = currentTime + scheduler->getQuantum();
        e = new Event(evtTimestamp, proc, TRANS_TO_PREEMPT, proc->processState,STATE_READY);
    } else {
        if (proc->curRemainingTime <= proc->curCPUburst) {
            evtTimestamp = currentTime + proc->curRemainingTime;
            e = new Event(evtTimestamp, proc, TRANS_TO_DONE, proc->processState,STATE_DONE);
        } else {
            evtTimestamp = currentTime + proc->curCPUburst;
            e = new Event(evtTimestamp, proc, TRANS_TO_BLOCK, proc->processState,STATE_BLOCKED);
        }
    }
    evtQueue->putEvent(e);
}

void handleTransToBlock(Event *evt) {
    Process *proc = evt->process; // this is the process the event works on
    int currentTime = evt->evtTimeStamp;
    int timeInPrevState = currentTime - proc->stateTs;
    // prevState: Running
    // nextState: Ready

    proc->processState = STATE_BLOCKED;
    if (evt->oldState == STATE_RUNNING) {
        proc->curIOburst = getRandom(proc->getIOburst()); // transition 3 : Running > Blocked
        proc->IOtime += proc->curIOburst;
    }
    proc->curCPUburst = 0;
    proc->timeInPrevState = timeInPrevState;
    proc->curRemainingTime = proc->curRemainingTime - proc->timeInPrevState;
    proc->stateTs = currentTime;
    if (evt->oldState == STATE_READY) {
        proc->CPUwaitingTime += proc->timeInPrevState;
    }

    if (isDoingIO && (currentTime > finishIOtime)) {
        timeIObusy += (finishIOtime - startIOtime);
        isDoingIO = false;
    } else if (currentTime + proc->curIOburst > finishIOtime) {
        finishIOtime = currentTime + proc->curIOburst;
    }

    if (!isDoingIO) {
        startIOtime = currentTime;
        finishIOtime = currentTime + proc->curIOburst;
        isDoingIO = true;
    }

    Event *e = new Event(currentTime + proc->curIOburst, proc, TRANS_TO_READY, proc->processState, STATE_READY);
    evtQueue->putEvent(e);

    callScheduler = true;
    currentRunningProcess = nullptr;
}

void handleTransToPreempt(Event *evt) {
    Process *proc = evt->process; // this is the process the event works on
    int currentTime = evt->evtTimeStamp;
    int timeInPrevState = currentTime - proc->stateTs;
    // prevState: Running
    // nextState: Ready
    proc->dynamicPriority--;
    proc->processState = STATE_READY;
    proc->timeInPrevState = timeInPrevState;
    proc->curCPUburst -= scheduler->getQuantum();
    proc->curRemainingTime -= scheduler->getQuantum();
    proc->stateTs = currentTime;
    if (evt->oldState == STATE_READY) {
        proc->CPUwaitingTime += proc->timeInPrevState;
    }
    scheduler->addProcess(proc);

    callScheduler = true;
    currentRunningProcess = nullptr;
}

void handleTransToDone(Event *evt) {
    Process *proc = evt->process; // this is the process the event works on
    int currentTime = evt->evtTimeStamp;
    int timeInPrevState = currentTime - proc->stateTs;
    // prevState: Running
    // nextState: X

    proc->processState = STATE_DONE;
    proc->timeInPrevState = timeInPrevState;
    proc->finishingTime = currentTime;
    proc->curRemainingTime = 0;
    if (evt->oldState == STATE_READY) {
        proc->CPUwaitingTime += proc->timeInPrevState;
    }
    callScheduler = true;
    currentRunningProcess = nullptr;
}

void printStatistics() {
    scheduler->printAlgoInfo();

    int finishTime = -1;
    int timeCPUbusy = 0;
    double totalTurnaroundTime = 0;
    double totalCPUwaitTime = 0;

    for (deque<Process *>::iterator iter = results.begin(); iter != results.end(); iter++) {
        if (finishTime < (*iter)->finishingTime) {
            finishTime = (*iter)->finishingTime;
        }
        timeCPUbusy += (*iter)->getTotalCPUtime();
        totalTurnaroundTime += ((*iter)->finishingTime - (*iter)->getArrivalTime());
        totalCPUwaitTime += ((*iter)->CPUwaitingTime);
        printf("%04d: %4d %4d %4d %4d %1d | %5d %5d %5d %5d\n",
               (*iter)->getPID(), (*iter)->getArrivalTime(), (*iter)->getTotalCPUtime(), (*iter)->getCPUburst(), (*iter)->getIOburst(), (*iter)->staticPriority,
               (*iter)->finishingTime, (*iter)->finishingTime - (*iter)->getArrivalTime(), (*iter)->IOtime, (*iter)->CPUwaitingTime);
    }

    // calculate
    double cpuUtil = 100.0 * (timeCPUbusy / (double)finishTime);
    timeIObusy += (finishIOtime - startIOtime);
    double IOutil = 100.0 * (timeIObusy / (double)finishTime);
    double avgTurnaroundTime = totalTurnaroundTime / results.size();
    double avgWaitTime = totalCPUwaitTime / results.size();
    double throughput = 100.0 * (results.size() / (double)finishTime);

    printf("SUM: %d %.2lf %.2lf %.2lf %.2lf %.3lf\n",
           finishTime, cpuUtil, IOutil, avgTurnaroundTime, avgWaitTime, throughput);
}

void printVerbose(int currentTime, Event *evt) {
    if ((evt->oldState == STATE_CREATED) && (evt->newState == STATE_READY)) {
        cout << currentTime << " " << evt->process->getPID() << " " << evt->process->timeInPrevState << ": "
             << processStateToString(evt->oldState) << " -> " << processStateToString(evt->newState) << endl;
    } else if ((evt->oldState == STATE_RUNNING) && (evt->newState == STATE_READY)) {
        cout << currentTime << " " << evt->process->getPID() << " " << evt->process->timeInPrevState << ": "
             << processStateToString(evt->oldState) << " -> " << processStateToString(evt->newState)
             << "  cb=" << evt->process->curCPUburst << " rem=" << evt->process->curRemainingTime << " prio=" << evt->process->dynamicPriority << endl;
    } else if ((evt->oldState == STATE_BLOCKED) && (evt->newState == STATE_READY)) {
        cout << currentTime << " " << evt->process->getPID() << " " << evt->process->timeInPrevState << ": "
             << processStateToString(evt->oldState) << " -> " << processStateToString(evt->newState) << endl;
    } else if ((evt->oldState == STATE_READY) && (evt->newState == STATE_RUNNING)) {
        cout << currentTime << " " << evt->process->getPID() << " " << evt->process->timeInPrevState << ": "
             << processStateToString(evt->oldState) << " -> " << processStateToString(evt->newState)
             << " cb=" << evt->process->curCPUburst << " rem=" << evt->process->curRemainingTime << " prio=" << evt->process->dynamicPriority << endl;
    } else if ((evt->oldState == STATE_RUNNING) && (evt->newState == STATE_BLOCKED)) {
        cout << currentTime << " " << evt->process->getPID() << " " << evt->process->timeInPrevState << ": "
             << processStateToString(evt->oldState) << " -> " << processStateToString(evt->newState)
             << "  ib=" << evt->process->curIOburst << " rem=" << evt->process->curRemainingTime << endl;
    } else if (evt->process->processState == STATE_DONE) {
        cout << currentTime << " " << evt->process->getPID() << " " << evt->process->timeInPrevState << ": Done" << endl;
    }
}

void initEventQueue(string fileName, int maxprio) {
    ifstream input;
    input.open(fileName);
    if (!input) {
        cerr << "Cannot open file!" << endl;
        exit(1);
    }

    string line;
    const char delim[] = " \t\r\n\v\f";
    int pid = 0;
    while (getline(input, line)) {
        // convert string to char *
        char *copiedLine = new char[line.size() + 1];
        copy(line.begin(), line.end(), copiedLine);
        copiedLine[line.size()] = '\0';

        char *ptr = strtok(copiedLine, delim);
        int parsed[4];
        int i = 0;
        while (ptr != NULL) {
            parsed[i++] = atoi(ptr);
            ptr = strtok(NULL, delim);
        }
        Process *p = new Process(pid++, parsed[0], parsed[1], parsed[2], parsed[3], getRandom(maxprio));
        Event *evt = new Event(parsed[0], p, TRANS_TO_READY, STATE_CREATED, STATE_READY);
        evtQueue->putEvent(evt);
        results.push_back(p);
    }
    input.close();
}

void readRandomNums(string fileName) {
    ifstream input;
    input.open(fileName);
    if (!input) {
        cerr << "Cannot open file!" << endl;
        exit(1);
    }

    string line;
    getline(input, line);
    int cnt = stoi(line);

    for (int i = 0; i < cnt; i++) {
        getline(input, line);
        randomNums.push_back(stoi(line));
    }

    input.close();
}

// transitions 2 and 3 (with noted exceptions) and the initial assignment of the static priority.
int getRandom(int burst) {
    int num = 1 + (randomNums[ofs++] % burst);
    if (ofs == randomNums.size()) {
        ofs = 0;
    }
    return num;
}