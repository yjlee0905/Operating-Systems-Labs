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
        return -1; // TODO check what happens when it returns -1

    return eventQ.front()->evtTimeStamp;
}

int EventQueue::getEventTimeWithProcess(Process *p) {
    if (p == nullptr) return -1;
    for (deque<Event*>::iterator iter = eventQ.begin(); iter != eventQ.end(); iter++) {
        if ((*iter)->process->getPID() == p->getPID()) {
            return (*iter)->evtTimeStamp;
        }
    }
    return -1;
}

void EventQueue::removeEvent(Process *p, int currentTime, int quantum) {
    for (deque<Event*>::iterator iter = eventQ.begin(); iter != eventQ.end(); iter++) {
        if ((*iter)->process->getPID() == p->getPID()) {
            p->curRemainingTime = (*iter)->process->curRemainingTime - (currentTime - (*iter)->process->stateTs)+quantum;
            p->curCPUburst = (*iter)->process->curCPUburst - (currentTime - p->stateTs)+quantum;
            eventQ.erase(iter);
            break;
        }
    }
}
