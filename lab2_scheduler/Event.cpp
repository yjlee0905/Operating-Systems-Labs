//
// Created by Yunjeon Lee on 2021/07/02.
//

#include "Event.h"

Event::Event(int evtTimeStamp, Process* process, trans transition, process_state_t oldState, process_state_t newState) {
    this->evtTimeStamp = evtTimeStamp;
    this->process = process;
    this->oldState = oldState;
    this->newState = newState;
    this->transition = transition;
}


// EventQueue
Event* EventQueue::getEvent() {
    if (eventQ.empty())
        return nullptr;

    Event* front = eventQ.front();
    eventQ.pop_front();
    return front;
}

void EventQueue::putEvent(Event* newEvent) {
    // auto?
    int size = eventQ.size();
    for (deque<Event*>::iterator iter = eventQ.begin(); iter != eventQ.end(); iter++) {
        if (newEvent->evtTimeStamp < (*iter)->evtTimeStamp) {
            eventQ.insert(iter, newEvent);
            break;
        }
    }

    if (eventQ.size() == size) {
        eventQ.push_back(newEvent);
    }
}

int EventQueue::getNextEventTime() {
    if (eventQ.empty())
        return -1;

    return eventQ.front()->evtTimeStamp;
}
