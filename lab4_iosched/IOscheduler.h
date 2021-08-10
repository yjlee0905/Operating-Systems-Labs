//
// Created by Yunjeon Lee on 2021/08/05.
//

#ifndef OPERATING_SYSTEMS_LABS_IOSCHEDULER_H
#define OPERATING_SYSTEMS_LABS_IOSCHEDULER_H

#include <iostream>
#include <deque>
#include "IOreq.h"

using namespace std;

class IOscheduler {
public:
    virtual void addIOrequest(IOreq* req) = 0;
    virtual IOreq* getNextIOrequest(int pos) = 0;
    //virtual int showNextIOreqArrivaltime() = 0;
    virtual bool isIOqueueEmpty() = 0;
};

class FIFOiosched : public IOscheduler {
private:
    deque<IOreq*> IOreqQ;

public:
    FIFOiosched();
    void addIOrequest(IOreq* req);
    IOreq* getNextIOrequest(int pos);
    //int showNextIOreqArrivaltime();
    bool isIOqueueEmpty();
};

class SSTFiosched : public IOscheduler {
private:
    deque<IOreq*> IOreqQ;

public:
    SSTFiosched();
    void addIOrequest(IOreq* req);
    IOreq* getNextIOrequest(int pos);
    bool isIOqueueEmpty();
};

#endif //OPERATING_SYSTEMS_LABS_IOSCHEDULER_H