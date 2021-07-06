//
// Created by Yunjeon Lee on 2021/07/03.
//

#include <iostream>
#include "Scheduler.h"

using namespace std;

Scheduler::Scheduler(int quantum){
    this->quantum = quantum;
}

void Scheduler::addProcess(Process *p) {}
Process* Scheduler::getNextProcess(){}
int Scheduler::getQuantum(){}
int Scheduler::getProcessCount(){}
void Scheduler::showSchedulerStatus(){}

FCFSsched::FCFSsched(int quantum) : Scheduler(quantum) {
    this->quantum = quantum;
}

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

int FCFSsched::getQuantum(){return quantum;}

int FCFSsched::getProcessCount() {
    return runQ.size();
}

void FCFSsched::showSchedulerStatus(){
    cout << "SCHED (" << runQ.size() << "): ";
    for (int i=0; i<runQ.size(); i++) {
        cout << " "<< runQ.at(i)->getPID() << ":" << runQ.at(i)->stateTs;
    }
    cout << endl;
}


LCFSsched::LCFSsched(int quantum) : Scheduler(quantum) {
    this->quantum = quantum;
}

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

int LCFSsched::getQuantum(){return quantum;}

int LCFSsched::getProcessCount() {
    return runQ.size();
}

void LCFSsched::showSchedulerStatus(){
    cout << "SCHED (" << runQ.size() << "): ";
    for (int i=0; i<runQ.size(); i++) {
        cout << " "<< runQ.at(i)->getPID() << ":" << runQ.at(i)->stateTs;
    }
    cout << endl;
}


SRTFsched::SRTFsched(int quantum) : Scheduler(quantum) {
    this->quantum = quantum;
}

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

int SRTFsched::getQuantum(){return quantum;}

int SRTFsched::getProcessCount() {
    return runQ.size();
}

void SRTFsched::showSchedulerStatus(){
    cout << "SCHED (" << runQ.size() << "): ";
    for (int i=0; i<runQ.size(); i++) {
        cout << " "<< runQ.at(i)->getPID() << ":" << runQ.at(i)->stateTs;
    }
    cout << endl;
}


RRsched::RRsched(int quantum) : Scheduler(quantum) {
    this->quantum = quantum;
}

void RRsched::addProcess(Process* p){
    runQ.push_back(p);
}

Process* RRsched::getNextProcess(){
    Process* p;
    if (!runQ.empty()){
        p = runQ.front();
        runQ.pop_front();
        return p;
    } else {
        return nullptr;
    }
}

int RRsched::getQuantum(){return quantum;}

int RRsched::getProcessCount() {
    return runQ.size();
}

void RRsched::showSchedulerStatus(){
    cout << "SCHED (" << runQ.size() << "): ";
    for (int i=0; i<runQ.size(); i++) {
        cout << " "<< runQ.at(i)->getPID() << ":" << runQ.at(i)->stateTs;
    }
    cout << endl;
}