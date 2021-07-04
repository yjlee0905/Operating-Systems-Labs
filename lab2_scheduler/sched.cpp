//
// Created by Yunjeon Lee on 2021/06/28.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <string>
#include "Process.h"
#include "Event.h"
#include "Scheduler.h"

using namespace std;

void initEventQueue(string fileName);
void readRandomNums(string fileName);
int getRandom(int burst);
void printVerbose(int currentTime, Event* evt);

EventQueue* evtQueue = new EventQueue(); // TODO 포인터 제거?
queue<Process*> results;
vector<int> randomNums; // max : 4611686018427387903(built as 64-bit target), 1073741823(built as 32-bit target)
int ofs = 0;

int main() {
    string inputFileName = "/Users/yjeonlee/Desktop/Operating_Systems/Operating-Systems-Labs/lab2_scheduler/inputs/input0";
    string rFileName = "/Users/yjeonlee/Desktop/Operating_Systems/Operating-Systems-Labs/lab2_scheduler/rfile";

    readRandomNums(rFileName);
    initEventQueue(inputFileName);

    Scheduler* scheduler = new FCFSsched();
    bool callScheduler = false;
    Process* currentRunningProcess = nullptr;

    // simulation
    while (!evtQueue->eventQ.empty()) {
        Event* evt = evtQueue->eventQ.front();
        evtQueue->eventQ.pop_front();

        Process* proc = evt->process; // this is the process the event works on
        int currentTime = evt->evtTimeStamp;
        int timeInPrevState = currentTime - proc->stateTs;

        switch (evt->transition) {
            case TRANS_TO_READY:
                // prevState: Created, Running, Blocked, + Preempted
                // nextState: Running

                // Ready state
                proc->processState = STATE_READY;
                proc->curCPUburst = 0;
                proc->curIOburst = 0;
                proc->timeInPrevState = timeInPrevState;
                proc->stateTs = currentTime;
                evt->oldState = STATE_READY;
                evt->newState = STATE_RUNNING;
                evt->transition = TRANS_TO_RUN;
                // evt->evtTimeStamp ?
                scheduler->addProcess(proc);
                callScheduler = true;
                break;
            case TRANS_TO_RUN:
                // prevState: Ready
                // nextState: Ready, Blocked

                proc->processState = STATE_RUNNING;
                proc->curCPUburst = getRandom(proc->getTotalCPUburst()); // transition 2: Read > Running 일 때만
                if (proc->curCPUburst > proc->curRemainingTime) {
                    proc->curCPUburst = proc->curRemainingTime;
                }
                proc->curRemainingTime = proc->getTotalCPUburst() - proc->curCPUburst;
                proc->curIOburst = 0;

                proc->timeInPrevState = timeInPrevState;
                proc->stateTs = currentTime;

                evt->oldState = STATE_RUNNING;
                evt->newState = STATE_BLOCKED;
                evt->transition = TRANS_TO_BLOCK;
                // evt->evtTimeStamp ?

                if (proc->curRemainingTime <= proc->curCPUburst) {
                    int timestamp = currentTime + proc->curRemainingTime;
                    Event* e = new Event(timestamp, proc, TRANS_TO_DONE, proc->processState, STATE_DONE);
                    evtQueue->putEvent(e);
                } else {
                    int timestamp = currentTime + proc->curCPUburst;
                    Event *e = new Event(timestamp, proc, TRANS_TO_BLOCK, proc->processState, STATE_BLOCKED);
                    evtQueue->putEvent(e);
                }
                printVerbose(currentTime, evt);
                break;
            case TRANS_TO_BLOCK:
                // prevState: Running
                // nextState: Ready
                printVerbose(currentTime, evt);

                proc->processState = STATE_BLOCKED;
                proc->stateTs = currentTime;
                proc->curIOburst = getRandom(proc->getTotalIOburst()); // transition 3 : Running > Blocked 일 때만

                // save prev state
                proc->timeInPrevState = proc->curIOburst;

                evt->oldState = STATE_BLOCKED;

                Event *e = new Event(currentTime + proc->curIOburst, proc, TRANS_TO_READY, proc->processState, STATE_READY);
                evtQueue->putEvent(e);
                callScheduler = true;
                currentRunningProcess = nullptr;
                callScheduler = true;
                break;
//            case TRANS_TO_PREEMPT :
//                //proc->processState =
//                printVerbose(currentTime, evt);
//
//                proc->stateTs = currentTime;
//                callScheduler = true;
//                break;
        }

        // remove current event object from Memory
        delete evt;
        evt = nullptr;

        if (callScheduler) {
            if (evtQueue->getNextEventTime() == currentTime) {
                continue; // process next event from Event Queue
            }
            callScheduler = false;
            if (currentRunningProcess == nullptr) {
                currentRunningProcess = scheduler->getNextProcess();
                if (currentRunningProcess == nullptr) continue;

                // create event to make process runnable for same time.
                Event* e = new Event(currentTime, currentRunningProcess, TRANS_TO_RUN, currentRunningProcess->processState, STATE_RUNNING);
                evtQueue->putEvent(e);
            }
        }
    }
}

void printVerbose(int currentTime, Event* evt) {
    if ((processStateToString(evt->oldState).compare("CREATED") == 0) && (processStateToString(evt->newState).compare("READY") == 0)) {
        cout << currentTime << " " << evt->process->getPID() << " " << evt->process->timeInPrevState << ": "
             << processStateToString(evt->oldState) << " -> " << processStateToString(evt->newState) << endl;
    } else if ((processStateToString(evt->oldState) == "READY") && (processStateToString(evt->newState) == "RUNNG")) {
        cout << currentTime << " " << evt->process->getPID() << " " << evt->process->timeInPrevState << ": "
             << processStateToString(evt->oldState) << " -> " << processStateToString(evt->newState)
             << " cb=" << evt->process->curCPUburst << " rem=" << evt->process->curRemainingTime
             << " prio=" << evt->process->dynamicPriority << endl;
    } else if ((processStateToString(evt->oldState) == "RUNNG") && (processStateToString(evt->newState) == "BLOCK")) {
        cout << currentTime << " " << evt->process->getPID() << " " << evt->process->timeInPrevState << ": "
             << processStateToString(evt->oldState) << " -> " << processStateToString(evt->newState)
             << "  ib=" << evt->process->curIOburst << " rem=" << evt->process->curRemainingTime << endl;
    }
}

void initEventQueue(string fileName) {
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
        char *copiedLine = new char[line.size()+1];
        copy(line.begin(), line.end(), copiedLine);
        copiedLine[line.size()] = '\0';

        char * ptr = strtok(copiedLine, delim);
        int parsed[4];
        int i=0;
        while (ptr != NULL) {
            parsed[i++] = atoi(ptr);
            ptr = strtok(NULL, delim);
        }
        Process* p = new Process(pid++, parsed[0], parsed[1], parsed[2], parsed[3], getRandom(DEFAULT_MAX_PRIO));
        Event* evt = new Event(parsed[0], p, TRANS_TO_READY, STATE_CREATED, STATE_READY);
        evtQueue->putEvent(evt);

        results.push(p);
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

    for (int i=0; i<cnt; i++) {
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