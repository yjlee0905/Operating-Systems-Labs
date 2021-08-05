//
// Created by Yunjeon Lee on 2021/08/05.
//

#include "IOreq.h"

IOreq::IOreq(int reqId, int timeStamp, int target) {
    this->reqId = reqId;
    this->timeStamp = timeStamp;
    this->target = target;
}
