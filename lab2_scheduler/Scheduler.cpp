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


PRIOsched::PRIOsched(int quantum, int maxPrios) : Scheduler(quantum) {
    this->quantum = quantum;
    this->maxPrios = maxPrios;

    this->activeQ = new deque<Process*>[maxPrios];
    this->expiredQ = new deque<Process*>[maxPrios];
}

void PRIOsched::addProcess(Process* p){
    // 여기서 dynamicPriority set
//    if (p->prevState == STATE_BLOCKED) {
//        p->dynamicPriority = p->staticPriority - 1;
//        //cout << "stat - 1" << endl;
//    } else if (p->prevState == STATE_RUNNING) {
//        //cout << "dp--" << endl;
//        p->dynamicPriority--;
//    }

    if (p->isExpired) {
        //p->dynamicPriority = p->staticPriority-1;
        // TODO check dynamicPriority range : [0 - staticPriority-1]
        (expiredQ+(p->dynamicPriority))->push_back(p);
        //cout<<"expiredQ success" << endl;

    } else {
        // TODO check dynamicPriority range
        //p->dynamicPriority--;
        (activeQ+(p->dynamicPriority))->push_back(p);
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
    //cout << "in next process" << endl;
    Process* p = nullptr;
    if (isMLQempty(activeQ)) {
        if (!isMLQempty(expiredQ)) {
            //cout << "switched queue" << endl;
            // flip activeQ and expiredQ
            deque<Process*> *tmp = activeQ;
            activeQ = expiredQ;
            expiredQ = tmp;

        } else {
            return nullptr;
        }
    }

    for (int i=maxPrios-1; i>=0; i--) {
        if (!(activeQ+i)->empty()) {
            p = (activeQ+i)->front();
            (activeQ+i)->pop_front();
            break;
        }
    }

//    cout << "[GET] activeQ: ";
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

//    for (int i=0; i<maxPrios; i++) {
//        if (!(activeQ+i)->empty()) {
//            //cout << "there: " << endl;
//            p = (activeQ+i)->front();
//            //cout << "here: " << p->staticPriority << endl;
//            (activeQ+i)->pop_front();
//            break;
//        }
//    }
    return p;
}

int PRIOsched::getQuantum(){return quantum;}

int PRIOsched::getProcessCount() {
    return -1;
}

void PRIOsched::showSchedulerStatus(){
//    cout << "SCHED (" << runQ.size() << "): ";
//    for (int i=0; i<runQ.size(); i++) {
//        cout << " "<< runQ.at(i)->getPID() << ":" << runQ.at(i)->stateTs;
//    }
//    cout << endl;
    cout << "test" << endl;
}

bool PRIOsched::isMLQempty(deque<Process *> *q) {
    for (int i=0; i<maxPrios; i++) {
        if (!(q+i)->empty()) {
            return false;
        }
    }
    return true;
}