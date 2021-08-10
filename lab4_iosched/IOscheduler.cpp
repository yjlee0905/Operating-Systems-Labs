//
// Created by Yunjeon Lee on 2021/08/05.
//

#include <cstdlib>
#include "IOscheduler.h"

FIFOiosched::FIFOiosched() {}

void FIFOiosched::addIOrequest(IOreq *req) {
    IOreqQ.push_back(req);
}

IOreq* FIFOiosched::getNextIOrequest(int pos) {
    if (!IOreqQ.empty()) {
        IOreq* ioreq = IOreqQ.front();
        IOreqQ.pop_front();
        return ioreq;
    }
    return nullptr;
}

//int FIFOiosched::showNextIOreqArrivaltime() {
//    if (!IOreqQ.empty()) {
//        return IOreqQ.front()->getArrivalTime();
//    }
//    return -1;
//}

bool FIFOiosched::isIOqueueEmpty() {
    if (IOreqQ.empty()) return true;
    return false;
}

SSTFiosched::SSTFiosched() {}

void SSTFiosched::addIOrequest(IOreq *req) {
    IOreqQ.push_back(req);
}

IOreq* SSTFiosched::getNextIOrequest(int pos) {
    IOreq* minIOreq = IOreqQ.front();
    deque<IOreq*>::iterator deletePos = IOreqQ.begin();
    for (deque<IOreq*>::iterator iter = IOreqQ.begin(); iter != IOreqQ.end(); iter++) {
        if (abs(pos - minIOreq->getTarget()) > abs(pos - (*iter)->getTarget())) {
            minIOreq = *iter;
            deletePos = iter;
        }
    }

    IOreqQ.erase(deletePos);
    return minIOreq;
}

bool SSTFiosched::isIOqueueEmpty() {
    if (IOreqQ.empty()) return true;
    return false;
}