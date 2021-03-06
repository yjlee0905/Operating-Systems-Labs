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
    IOscheduler();
    virtual void addIOrequest(IOreq* req) = 0;
    virtual IOreq* getNextIOrequest(int pos, bool direction) = 0;
    virtual bool isIOqueueEmpty() = 0;
};

class FIFOiosched : public IOscheduler {
private:
    deque<IOreq*> IOqueue;

public:
    FIFOiosched();
    void addIOrequest(IOreq* req);
    IOreq* getNextIOrequest(int pos, bool direction);
    bool isIOqueueEmpty();
};

class SSTFiosched : public IOscheduler {
private:
    deque<IOreq*> IOqueue;

public:
    SSTFiosched();
    void addIOrequest(IOreq* req);
    IOreq* getNextIOrequest(int pos, bool direction);
    bool isIOqueueEmpty();
};

class LOOKiosched : public IOscheduler {
private:
    deque<IOreq*> IOqueue;

public:
    LOOKiosched();
    void addIOrequest(IOreq* req);
    IOreq* getNextIOrequest(int pos, bool direction);
    bool isIOqueueEmpty();
};

class CLOOKiosched : public IOscheduler {
private:
    deque<IOreq*> IOqueue;

public:
    CLOOKiosched();
    void addIOrequest(IOreq* req);
    IOreq* getNextIOrequest(int pos, bool direction);
    bool isIOqueueEmpty();
};

class FLOOKiosched : public IOscheduler {
private:
    deque<IOreq*> addQ;
    deque<IOreq*> activeQ;

public:
    FLOOKiosched();
    void addIOrequest(IOreq* req);
    IOreq* getNextIOrequest(int pos, bool direction);
    bool isIOqueueEmpty();
};

#endif //OPERATING_SYSTEMS_LABS_IOSCHEDULER_H