//
// Created by Yunjeon Lee on 2021/07/03.
//

#include <iostream>
#include "Scheduler.h"

using namespace std;

Scheduler::Scheduler(){}

void Scheduler::addProcess(Process *p) {}

Process* Scheduler::getNextProcess(){}

int Scheduler::getProcessCount(){}

void Scheduler::showShedulerStatus(){}

FCFSsched::FCFSsched(){}

void FCFSsched::addProcess(Process* process){
    runQ.push_back(process);
}

Process* FCFSsched::getNextProcess(){
    Process* p;
    if (!runQ.empty()){
        p = runQ.front();
        runQ.pop_front();
    } else {
        return nullptr;
    }
    return p;
}

int FCFSsched::getProcessCount() {
    return runQ.size();
}


void FCFSsched::showShedulerStatus(){
    cout << "SCHED (" << runQ.size() << "): ";
    for (int i=0; i<runQ.size(); i++) {
        cout << " "<< runQ.at(i)->getPID() << ":" << runQ.at(i)->stateTs;
    }
    cout << endl;
}
