//
// Created by Yunjeon Lee on 2021/08/05.
//

#include "IOreq.h"

IOreq::IOreq(int reqId, int timestamp, int target) {
    this->reqId = reqId;
    this->timestamp = timestamp;
    this->target = target;
}

int IOreq::getReqId() {return this->reqId;}
int IOreq::getTimestamp() {return this->timestamp;}
int IOreq::getTarget() {return this->target;}