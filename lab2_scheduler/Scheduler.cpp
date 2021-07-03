//
// Created by Yunjeon Lee on 2021/07/03.
//

#include <iostream>
#include "Scheduler.h"

using namespace std;

Scheduler::Scheduler(){

}

void Scheduler::addProcess(Process *p) {}

Process* Scheduler::getNextProcess(){}

FCFS::FCFS(){

}

void FCFS::addProcess(Process* process){
    readyQ.push_back(process);
}

Process* FCFS::getNextProcess(){
    Process* p;
    if (!readyQ.empty()){
        p = readyQ.front();
        readyQ.pop_front();
    }
    return p;;
}
