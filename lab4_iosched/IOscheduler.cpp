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
    if (IOreqQ.size() == 1) {
        IOreq* ioreq = IOreqQ.front();
        IOreqQ.pop_front();
        return ioreq;
    }

    IOreq* minIOreq = IOreqQ.at(0);
    int offset = 0;
    for (int i = 1; i < IOreqQ.size(); i++) {
        if (abs(pos - minIOreq->getTarget()) > abs(pos - IOreqQ.at(i)->getTarget())) {
            minIOreq = IOreqQ.at(i);
            offset = i;
        }
    }

    IOreqQ.erase(IOreqQ.begin() + offset);
    return minIOreq;
}

bool SSTFiosched::isIOqueueEmpty() {
    if (IOreqQ.empty()) return true;
    return false;
}