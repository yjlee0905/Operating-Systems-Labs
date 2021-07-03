//
// Created by Yunjeon Lee on 2021/06/28.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include "Process.h"
#include "Event.h"
#include "Scheduler.h"

using namespace std;

void initEventQueue(string fileName);
void readRandomNums(string fileName);
int getRandom(int burst);
void printVerbose(int currentTime, Event* evt);

EventQueue evtQueue;
queue<Process*> results;
vector<int> randomNums; // max : 4611686018427387903(built as 64-bit target), 1073741823(built as 32-bit target)
int ofs = 0;

int main() {
    string inputFileName = "/Users/yjeonlee/Desktop/Operating_Systems/Operating-Systems-Labs/lab2_scheduler/inputs/input6";
    string rFileName = "/Users/yjeonlee/Desktop/Operating_Systems/Operating-Systems-Labs/lab2_scheduler/rfile";

    readRandomNums(rFileName);
    initEventQueue(inputFileName);

    // simulation
    while (!evtQueue.eventQ.empty()) {
        Event* evt = evtQueue.eventQ.front();
        evtQueue.eventQ.pop_front();
        Process* proc = evt->process; // this is the process the event works on
        int currentTime = evt->timeStamp;
        //int timeInPrevState = currentTime -

        switch (evt->transition) {
            case TRANS_TO_READY:
                proc->processState = STATE_READY;

                // save prev state
                proc->prevStateDuration = 0; // TODO change

                printVerbose(currentTime, evt);

                evt->oldState = STATE_READY;
                // set evt.newState
                break;
            case TRANS_TO_RUN:
                proc->processState = STATE_RUNNING;
                proc->curCPUburst = getRandom(proc->getTotalCPUburst());
                // TODO prio??

                // save prev state
                proc->curRemainingTime = proc->getTotalCPUburst() - proc->curCPUburst;
                proc->prevStateDuration = proc->curCPUburst;

                printVerbose(currentTime, evt);

                evt->oldState = STATE_RUNNING;
                // set evt.newState
                break;
            case TRANS_TO_BLOCK:
                proc->processState = STATE_BLOCKED;
                proc->curIOburst = getRandom(proc->getTotalIOburst());

                // save prev state
                proc->prevStateDuration = proc->curIOburst;

                printVerbose(currentTime, evt);

                evt->oldState = STATE_BLOCKED;
                // set evt.newState
                break;
            case TRANS_TO_PREEMPT:
                //proc->processState =
                break;
        }

        // remove current event object from Memory
        delete evt;
        evt = nullptr;
    }
}

void printVerbose(int currentTime, Event* evt) {
    cout << currentTime << " " << evt->process->getPID() << " " << evt->process->prevStateDuration << ": "
    << processStateToString(evt->oldState) << " -> " << processStateToString(evt->newState) << endl;
    // cb=8 rem=100 prio=1
    // ib=2 rem=92
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
        //cout << copiedLine << endl;

        char * ptr = strtok(copiedLine, delim);
        int parsed[4];
        int i=0;
        while (ptr != NULL) {
            //cout << ptr  << "     converted: " << atoi(ptr) << endl;
            parsed[i++] = atoi(ptr);
            ptr = strtok(NULL, delim);
        }
        Process* p = new Process(++pid, parsed[0], parsed[1], parsed[2], parsed[3]);
        Event* evt = new Event(parsed[0], p, STATE_CREATED, STATE_READY, TRANS_TO_READY);
        evtQueue.putEvent(evt);
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

int getRandom(int burst) {
    int num = 1 + (randomNums[ofs++] % burst);
    if (ofs == randomNums.size()) {
        ofs = 0;
    }
    return num;
}