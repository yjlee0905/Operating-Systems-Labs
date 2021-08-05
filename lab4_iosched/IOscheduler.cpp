//
// Created by Yunjeon Lee on 2021/08/05.
//

#include "IOscheduler.h"

FIFOiosched::FIFOiosched() {}

void FIFOiosched::addIOrequest(IOreq *req) {
    IOreqQ.push_back(req);
}

IOreq* FIFOiosched::getNextIOrequest() {
    if (!IOreqQ.empty()) {
        IOreq* ioreq = IOreqQ.front();
        IOreqQ.pop_front();
        return ioreq;
    }
    return nullptr;
}