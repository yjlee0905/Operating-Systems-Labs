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
        return p;
    } else {
        return nullptr;
    }
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


LCFSsched::LCFSsched(){}

void LCFSsched::addProcess(Process* process){
    runQ.push_back(process);
}

Process* LCFSsched::getNextProcess(){
    Process* p;
    if (!runQ.empty()){
        p = runQ.back();
        runQ.pop_back();
        return p;
    } else {
        return nullptr;
    }
}

int LCFSsched::getProcessCount() {
    return runQ.size();
}

void LCFSsched::showShedulerStatus(){
    cout << "SCHED (" << runQ.size() << "): ";
    for (int i=0; i<runQ.size(); i++) {
        cout << " "<< runQ.at(i)->getPID() << ":" << runQ.at(i)->stateTs;
    }
    cout << endl;
}


SRTFsched::SRTFsched(){}

void SRTFsched::addProcess(Process* p){

    int size = runQ.size();
    for (deque<Process*>::iterator iter = runQ.begin(); iter != runQ.end(); iter++) {
        if (p->curRemainingTime < (*iter)->curRemainingTime) {
            runQ.insert(iter, p);
            break;
        }
    }

    if (runQ.size() == size) {
        runQ.push_back(p);
    }
}

Process* SRTFsched::getNextProcess(){
    Process* p;
    if (!runQ.empty()){
        p = runQ.front();
        runQ.pop_front();
        return p;
    } else {
        return nullptr;
    }
}

int SRTFsched::getProcessCount() {
    return runQ.size();
}

void SRTFsched::showShedulerStatus(){
    cout << "SCHED (" << runQ.size() << "): ";
    for (int i=0; i<runQ.size(); i++) {
        cout << " "<< runQ.at(i)->getPID() << ":" << runQ.at(i)->stateTs;
    }
    cout << endl;
}