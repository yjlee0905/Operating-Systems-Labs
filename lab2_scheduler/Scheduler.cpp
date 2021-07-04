//
// Created by Yunjeon Lee on 2021/07/03.
//

#include "Scheduler.h"

using namespace std;

Scheduler::Scheduler(){}

void Scheduler::addProcess(Process *p) {}

Process* Scheduler::getNextProcess(){}

FCFSsched::FCFSsched(){}

void FCFSsched::addProcess(Process* process){
    runQ.push_back(process);
}

Process* FCFSsched::getNextProcess(){
    Process* p;
    if (!runQ.empty()){
        p = runQ.front();
        runQ.pop_front();
    }
    return p;
}
