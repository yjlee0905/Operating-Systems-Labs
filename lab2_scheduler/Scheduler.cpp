//
// Created by Yunjeon Lee on 2021/07/03.
//

#include <iostream>
#include "Scheduler.h"

using namespace std;

Scheduler::Scheduler(){
    cout << "test" << endl;
}

void addProcess(Process* p) {
    cout << "test1" << endl;
}

Process* getNextProcess() {
    cout << "test2" << endl;
}

FCFS::FCFS(){
    cout << "test3" << endl;
}

void FCFS::addProcess(Process* p) {
    readyQ.push_back(p);
}

Process* FCFS::getNextProcess(){
    if (readyQ.empty())
        return nullptr;

    Process* front = readyQ.front();
    readyQ.pop_front();
    return front;
}
