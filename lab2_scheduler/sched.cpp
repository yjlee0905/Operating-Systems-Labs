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
void handleTransToReady(Event* evt);
void handleTransToRun(Event* evt);
void handleTransToBlock(Event* evt);
void handleTransToDone(Event* evt);
void printStatistics(string schedAlgo);
void printVerbose(int currentTime, Event* evt);

EventQueue* evtQueue = new EventQueue(); // TODO 포인터 제거?
deque<Process*> results;
vector<int> randomNums; // max : 4611686018427387903(built as 64-bit target), 1073741823(built as 32-bit target)
int ofs = 0;

// scheduler
Scheduler* scheduler = new LCFSsched();
bool callScheduler = false;
Process* currentRunningProcess = nullptr;

int main() {
    string inputFileName = "/Users/yjeonlee/Desktop/Operating_Systems/Operating-Systems-Labs/lab2_scheduler/inputs/input6";
    string rFileName = "/Users/yjeonlee/Desktop/Operating_Systems/Operating-Systems-Labs/lab2_scheduler/rfile";

    readRandomNums(rFileName);
    initEventQueue(inputFileName);

    // simulation
    while (!evtQueue->eventQ.empty()) {
        Event* evt = evtQueue->eventQ.front();
        evtQueue->eventQ.pop_front();

        //Process* proc = evt->process; // this is the process the event works on
        int currentTime = evt->evtTimeStamp;

        switch (evt->transition) {
            case TRANS_TO_READY:
                handleTransToReady(evt);
                printVerbose(currentTime, evt);
                break;
            case TRANS_TO_RUN:
                handleTransToRun(evt);
                printVerbose(currentTime, evt);
                break;
            case TRANS_TO_BLOCK:
                handleTransToBlock(evt);
                printVerbose(currentTime, evt);
                break;
//            case TRANS_TO_PREEMPT :
//                //proc->processState =
//                printVerbose(currentTime, evt);
//
//                proc->stateTs = currentTime;
//                callScheduler = true;
//                break;
            case TRANS_TO_DONE:
                handleTransToDone(evt);
                printVerbose(currentTime, evt);
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
                if (currentRunningProcess == nullptr) continue;

//                cout << "Call Sched: " << currentTime << " " << tmp->process->getPID() << " " << tmp->process->timeInPrevState << ": "
//                     << processStateToString(tmp->oldState) << " -> " << processStateToString(tmp->newState) << endl;

                // create event to make process runnable for same time.
                Event* e = new Event(currentTime, currentRunningProcess, TRANS_TO_RUN, currentRunningProcess->processState, STATE_RUNNING);
                evtQueue->putEvent(e);
            }
        }
    }
    printStatistics("FCFS");
}

void handleTransToReady(Event* evt) {
    Process* proc = evt->process; // this is the process the event works on
    int currentTime = evt->evtTimeStamp;
    int timeInPrevState = currentTime - proc->stateTs;

    // READY로 바뀌는 순간
    // prevState: Created, Running, Blocked, + Preempted
    // nextState: Running

    // Ready state
    proc->processState = STATE_READY;
    proc->curCPUburst = 0;
    proc->curIOburst = 0;
    //proc->curRemainingTime
    proc->timeInPrevState = timeInPrevState;
    proc->stateTs = currentTime;

//    Event* newEvt = new Event(currentTime, proc, TRANS_TO_RUN, STATE_READY, STATE_RUNNING);
//    evtQueue->putEvent(newEvt);

    //cout << "insert to sched" << endl;
    scheduler->addProcess(proc);
    callScheduler = true;
}

void handleTransToRun(Event* evt) {
    Process* proc = evt->process; // this is the process the event works on
    int currentTime = evt->evtTimeStamp;
    int timeInPrevState = currentTime - proc->stateTs;

    // prevState: Ready
    // nextState: Ready, Blocked

    proc->processState = STATE_RUNNING;
    if (evt->oldState == STATE_READY) {
        proc->curCPUburst = getRandom(proc->getCPUburst()); // transition 2: Read > Running 일 때만
        if (proc->curCPUburst > proc->curRemainingTime) {
            proc->curCPUburst = proc->curRemainingTime;
        }
    }
    //proc->curRemainingTime = proc->curRemainingTime - proc->curCPUburst;
    proc->curIOburst = 0;
    proc->timeInPrevState = timeInPrevState;
    proc->stateTs = currentTime;

    if (proc->curRemainingTime <= proc->curCPUburst) {
        int evtTimestamp = currentTime + proc->curRemainingTime;
        Event* e = new Event(evtTimestamp, proc, TRANS_TO_DONE, proc->processState, STATE_DONE);
        evtQueue->putEvent(e);
    } else {
        int evtTimestamp = currentTime + proc->curCPUburst;
        Event *e = new Event(evtTimestamp, proc, TRANS_TO_BLOCK, proc->processState, STATE_BLOCKED);
        evtQueue->putEvent(e);
    }
    // TODO TRANS_TO_PREEMPT
}

void handleTransToBlock(Event* evt) {
    Process* proc = evt->process; // this is the process the event works on
    int currentTime = evt->evtTimeStamp;
    int timeInPrevState = currentTime - proc->stateTs;
    // prevState: Running
    // nextState: Ready

    proc->processState = STATE_BLOCKED;
    if (evt->oldState == STATE_RUNNING) {
        proc->curIOburst = getRandom(proc->getIOburst()); // transition 3 : Running > Blocked 일 때만
        proc->IOtime += proc->curIOburst;
    }
    proc->curCPUburst = 0;
    proc->timeInPrevState = timeInPrevState;
    proc->curRemainingTime = proc->curRemainingTime - proc->timeInPrevState;
    proc->stateTs = currentTime;

    Event *e = new Event(currentTime + proc->curIOburst, proc, TRANS_TO_READY, proc->processState, STATE_READY);
    evtQueue->putEvent(e);

    callScheduler = true;
    currentRunningProcess = nullptr;
}

void handleTransToDone(Event* evt) {
    Process* proc = evt->process; // this is the process the event works on
    int currentTime = evt->evtTimeStamp;
    int timeInPrevState = currentTime - proc->stateTs;
    // prevState: Running
    // nextState: ?

    proc->processState = STATE_DONE;
    proc->timeInPrevState = timeInPrevState;
    proc->finishingTime = currentTime;
    proc->curRemainingTime = 0;
    callScheduler = true;
    currentRunningProcess = nullptr;
}

void printStatistics(string schedAlgo) {
    // TODO CPUwaitingTime, IOUtil
    cout << schedAlgo << endl;

    int finishTime = -1;
    int timeCPUbusy = 0;
    int timeIObusy = 0;
    double totalTurnaroundTime = 0;

    for (deque<Process*>::iterator iter = results.begin(); iter != results.end(); iter++) {
        if (finishTime < (*iter)->finishingTime) {
            finishTime = (*iter)->finishingTime;
        }
        timeCPUbusy += (*iter)->getTotalCPUtime();
        totalTurnaroundTime += ((*iter)->finishingTime - (*iter)->getArrivalTime());
        printf("%04d: %4d %4d %4d %4d %1d | %5d %5d %5d %5d\n",
               (*iter)->getPID(), (*iter)->getArrivalTime(), (*iter)->getTotalCPUtime(), (*iter)->getCPUburst(), (*iter)->getIOburst(), (*iter)->staticPriority,
               (*iter)->finishingTime, (*iter)->finishingTime - (*iter)->getArrivalTime(), (*iter)->IOtime, (*iter)->CPUwaitingTime);
    }

    // calculate
    double cpuUtil = 100.0 * (timeCPUbusy / (double) finishTime);
    //double IOutil = 100.0 * ()
    double aveTurnaroundTime = totalTurnaroundTime / results.size();
    double throughput = 100.0 * (results.size() / (double) finishTime);

    printf("SUM: %d %.2lf %.2lf %.2lf %.2lf %.3lf\n",
           finishTime, cpuUtil, 0.0, aveTurnaroundTime, 0.0, throughput);
}

void printVerbose(int currentTime, Event* evt) {
    if ((evt->oldState == STATE_CREATED) && (evt->newState == STATE_READY)) {
        cout << currentTime << " " << evt->process->getPID() << " " << evt->process->timeInPrevState << ": "
             << processStateToString(evt->oldState) << " -> " << processStateToString(evt->newState) << endl;
    } else if ((evt->oldState == STATE_BLOCKED) && (evt->newState == STATE_READY)) {
        cout << currentTime << " " << evt->process->getPID() << " " << evt->process->timeInPrevState << ": "
             << processStateToString(evt->oldState) << " -> " << processStateToString(evt->newState) << endl;
    } else if ((evt->oldState == STATE_READY) && (evt->newState == STATE_RUNNING)) {
        cout << currentTime << " " << evt->process->getPID() << " " << evt->process->timeInPrevState << ": "
             << processStateToString(evt->oldState) << " -> " << processStateToString(evt->newState)
             << " cb=" << evt->process->curCPUburst << " rem=" << evt->process->curRemainingTime
             << " prio=" << evt->process->dynamicPriority << endl;
    } else if ((evt->oldState == STATE_RUNNING) && (evt->newState == STATE_BLOCKED)) {
        cout << currentTime << " " << evt->process->getPID() << " " << evt->process->timeInPrevState << ": "
             << processStateToString(evt->oldState) << " -> " << processStateToString(evt->newState)
             << "  ib=" << evt->process->curIOburst << " rem=" << evt->process->curRemainingTime << endl;
    } else if (evt->process->processState == STATE_DONE) {
        cout << currentTime << " " << evt->process->getPID() << " " << evt->process->timeInPrevState << ": Done" << endl;
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