//
// Created by Yunjeon Lee on 2021/08/05.
//

#ifndef OPERATING_SYSTEMS_LABS_IOREQ_H
#define OPERATING_SYSTEMS_LABS_IOREQ_H


class IOreq {
private:
    int reqId;
    int timestamp;
    int target;

public:
    IOreq(int reqId, int timestamp, int target);
    int getReqId();
    int getTimestamp();
    int getTarget();
};

#endif //OPERATING_SYSTEMS_LABS_IOREQ_H