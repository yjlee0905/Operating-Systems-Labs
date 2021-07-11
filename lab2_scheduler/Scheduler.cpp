//
// Created by Yunjeon Lee on 2021/07/03.
//

#include <iostream>
#include "Scheduler.h"

using namespace std;

Scheduler::Scheduler(int quantum){
    this->quantum = quantum;
}

bool Scheduler::shouldPreempt(Process* curRunningProc, Process* proc, int evtTime, int currentTime) {
    return false;
}

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


void FCFSsched::printAlgoInfo() {
    cout << "FCFS" << endl;
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

void LCFSsched::printAlgoInfo() {
    cout << "LCFS" << endl;
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

void SRTFsched::printAlgoInfo() {
    cout << "SRTF" << endl;
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

void RRsched::printAlgoInfo() {
    cout << "RR" << " " << getQuantum() << endl;
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


PRIOsched::PRIOsched(int quantum, int maxPrios) : Scheduler(quantum) {
    this->quantum = quantum;
    this->maxPrios = maxPrios;

    activeQ = q1;
    expiredQ = q2;
    //expiredQ = new deque<Process*>[maxPrios];
}

void PRIOsched::addProcess(Process* p){

    if (p->dynamicPriority >= 0) {
        activeQ[p->dynamicPriority].push_back(p);
    } else {
        p->dynamicPriority = p->staticPriority - 1;
        expiredQ[p->dynamicPriority].push_back(p);
    }
//    cout << "[ADD] activeQ: ";
//    for (int i=0; i<maxPrios; i++) {
//        cout << "[ ";
//        for (deque<Process*>::iterator iter = (activeQ+i)->begin(); iter != (activeQ+i)->end(); iter++) {
//            cout << (*iter)->getPID() <<  ", ";
//        }
//        cout << "] ";
//    }
//
//    cout << "          expiredQ: ";
//    for (int i=0; i<maxPrios; i++) {
//        cout << "[ ";
//        for (deque<Process*>::iterator iter = (expiredQ+i)->begin(); iter != (expiredQ+i)->end(); iter++) {
//            cout << (*iter)->getPID() <<  ", ";
//        }
//        cout << "] ";
//    }
//    cout << endl;
}

Process* PRIOsched::getNextProcess(){
    Process* p = nullptr;
    for (int j=0; j<2; j++) {
        for (int i=maxPrios-1; i>=0; i--) {
            if (!(activeQ+i)->empty()) {
                p = (activeQ+i)->front();
                (activeQ+i)->pop_front();
                return p;
            }
        }
        deque<Process*> *tmp = activeQ;
        activeQ = expiredQ;
        expiredQ = tmp;
    }
    return nullptr;
}

void PRIOsched::printAlgoInfo() {
    cout << "PRIO" << " " << getQuantum() << endl;
}

int PRIOsched::getQuantum(){return quantum;}

int PRIOsched::getProcessCount() {
    // TODO implement
    return -1;
}

void PRIOsched::showSchedulerStatus(){
    // TODO implement
}


PREPRIOsched::PREPRIOsched(int quantum, int maxPrios) : PRIOsched(quantum, maxPrios) {
    this->quantum = quantum;
    this->maxPrios = maxPrios;

    activeQ = q1;
    expiredQ = q2;
}

void PREPRIOsched::printAlgoInfo() {
    cout << "PREPRIO" << " " << getQuantum() << endl;
}

bool PREPRIOsched::shouldPreempt(Process* curRunningProc, Process* proc, int evtTime, int currentTime) {
    //cout << "---> PRIO preemption " << curRunningProc->getPID() << " by " << proc->getPID() << " ?";
    if ((evtTime != currentTime) && (proc->dynamicPriority > curRunningProc->dynamicPriority)) {
        //cout << " " << proc->getPID() << " TS=" << evtTime << " now=" << currentTime<<") --> YES" << endl;
        return true;
    }
    //cout << " " << curRunningProc->getPID() << " TS=" << evtTime << " now=" << currentTime<<") --> NO" << endl;
    return false;
}