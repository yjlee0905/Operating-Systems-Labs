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
private:
    process_state_t oldState;
    process_state_t newState;
public:
    Process* process;
    trans transition;
    int timeStamp;

    Event(Process* process, process_state_t oldState, process_state_t newState);
};

class EventQueue { // TODO EventQueue는 Deque으로??
public:
    deque<Event*> eventQ;

    Event* getEvent();
    void putEvent(Event* newEvent);
    // TODO rm_event();
};

#endif //OPERATING_SYSTEMS_LABS_EVENT_H