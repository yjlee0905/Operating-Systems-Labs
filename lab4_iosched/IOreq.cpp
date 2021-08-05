//
// Created by Yunjeon Lee on 2021/08/05.
//

#include "IOreq.h"

IOreq::IOreq(int reqId, int timestamp, int target) {
    this->reqId = reqId;
    this->arrivalTime = timestamp;
    this->target = target;
    this->start = -1;
    this->end = -1;
}

int IOreq::getReqId() {return this->reqId;}
int IOreq::getArrivalTime() {return this->arrivalTime;}
int IOreq::getTarget() {return this->target;}