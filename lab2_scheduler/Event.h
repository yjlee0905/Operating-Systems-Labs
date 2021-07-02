//
// Created by Yunjeon Lee on 2021/07/02.
//

#ifndef OPERATING_SYSTEMS_LABS_EVENT_H
#define OPERATING_SYSTEMS_LABS_EVENT_H

#include <iostream>
#include <deque>
#include "Process.h"

using namespace std;

typedef enum {
    STATE_CREATED,
    STATE_READY,
    STATE_RUNNING,
    STATE_BLOCKED
} process_state_t;

class Event {
private:
    Process* process;
    process_state_t oldState;
    process_state_t newState;
public:
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