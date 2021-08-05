//
// Created by Yunjeon Lee on 2021/08/05.
//

#ifndef OPERATING_SYSTEMS_LABS_IOREQ_H
#define OPERATING_SYSTEMS_LABS_IOREQ_H


class IOreq {
private:
    int reqId;
    int timeStamp;
    int target;

public:
    IOreq(int reqId, int timeStamp, int target);
};


#endif //OPERATING_SYSTEMS_LABS_IOREQ_H
