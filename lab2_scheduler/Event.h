//
// Created by Yunjeon Lee on 2021/07/02.
//

#ifndef OPERATING_SYSTEMS_LABS_EVENT_H
#define OPERATING_SYSTEMS_LABS_EVENT_H

#include <iostream>
#include <deque>
#include "Constants.h"
#include "Process.h"

using namespace std;

class Event {
public:
    Process* process;
    process_state_t oldState;
    process_state_t newState;
    trans transition;
    int evtTimeStamp;

    Event(int evtTimeStamp, Process* process, trans transition, process_state_t oldState, process_state_t newState);
};

class EventQueue { // TODO EventQueue는 Deque으로??
public:
    deque<Event*> eventQ;

    Event* getEvent();
    void putEvent(Event* newEvent);
    int getNextEventTime();
    int getEventTimeWithProcess(Process* p);
    void removeEvent(Process* p, int currentTime, int quantum);
};

#endif //OPERATING_SYSTEMS_LABS_EVENT_H