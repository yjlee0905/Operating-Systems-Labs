//
// Created by Yunjeon Lee on 2021/08/05.
//

#ifndef OPERATING_SYSTEMS_LABS_IOREQ_H
#define OPERATING_SYSTEMS_LABS_IOREQ_H


class IOreq {
private:
    int reqId;
    int arrivalTime;
    int target;

public:
    int start;
    int end;

    IOreq(int reqId, int arrivalTime, int target);
    int getReqId();
    int getArrivalTime();
    int getTarget();
};

#endif //OPERATING_SYSTEMS_LABS_IOREQ_H