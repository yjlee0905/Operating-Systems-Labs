//
// Created by Yunjeon Lee on 2021/08/05.
//

#include <cstdlib>
#include <climits>
#include "IOscheduler.h"

using namespace std;

IOscheduler::IOscheduler() {}

FIFOiosched::FIFOiosched() {}

void FIFOiosched::addIOrequest(IOreq *req) {
    IOqueue.push_back(req);
}

IOreq* FIFOiosched::getNextIOrequest(int pos, bool direction) {
    if (!IOqueue.empty()) {
        IOreq* ioreq = IOqueue.front();
        IOqueue.pop_front();
        return ioreq;
    }
    return nullptr;
}

bool FIFOiosched::isIOqueueEmpty() {
    if (IOqueue.empty()) return true;
    return false;
}

SSTFiosched::SSTFiosched() {}

void SSTFiosched::addIOrequest(IOreq *req) {
    IOqueue.push_back(req);
}

IOreq* SSTFiosched::getNextIOrequest(int pos, bool direction) {
    if (IOqueue.size() == 1) {
        IOreq* ioreq = IOqueue.front();
        IOqueue.pop_front();
        return ioreq;
    }

    IOreq* minIOreq = IOqueue.at(0);
    int offset = 0;
    for (int i = 1; i < IOqueue.size(); i++) {
        if (abs(pos - minIOreq->getTarget()) > abs(pos - IOqueue.at(i)->getTarget())) {
            minIOreq = IOqueue.at(i);
            offset = i;
        }
    }

    IOqueue.erase(IOqueue.begin() + offset);
    return minIOreq;
}

bool SSTFiosched::isIOqueueEmpty() {
    if (IOqueue.empty()) return true;
    return false;
}

LOOKiosched::LOOKiosched() {}

void LOOKiosched::addIOrequest(IOreq* req) {
    int size = IOqueue.size();
    for (deque<IOreq*>::iterator iter = IOqueue.begin(); iter != IOqueue.end(); iter++) {
        if (req->getTarget() < (*iter)->getTarget()) {
            IOqueue.insert(iter, req);
            break;
        }
    }

    if (IOqueue.size() == size) {
        IOqueue.push_back(req);
    }
}

IOreq* LOOKiosched::getNextIOrequest(int pos, bool direction) {
    if (direction) { // head is in direction of increment
        if (IOqueue.back()->getTarget() <= pos) {
            IOreq* nextIOreq = IOqueue.back();
            int offset = IOqueue.size() - 1;
            for (int i = 0; i < IOqueue.size(); ++i) {
                if (IOqueue.at(i)->getTarget() == nextIOreq->getTarget()) {
                    nextIOreq = IOqueue.at(i);
                    offset = i;
                    break;
                }
            }

            IOqueue.erase(IOqueue.begin() + offset);
            return nextIOreq;
        } else {
            if (IOqueue.size() == 1) {
                IOreq* ioreq = IOqueue.front();
                IOqueue.pop_front();
                return ioreq;
            }

            IOreq* minDistReq = nullptr;
            int minDist = INT_MAX;
            int offset = 0;
            for (int i = 0; i < IOqueue.size(); i++) {
                if (pos <= IOqueue.at(i)->getTarget()) {
                    int dist = abs(IOqueue.at(i)->getTarget() - pos);
                    if (dist < minDist) {
                        minDist = dist;
                        minDistReq = IOqueue.at(i);
                        offset = i;
                    }
                }

            }

            IOqueue.erase(IOqueue.begin() + offset);
            return minDistReq;
        }
    } else { // head is in direction of decrement
        if (IOqueue.front()->getTarget() >= pos) { // change direction
            IOreq* nextIOreq = IOqueue.front();
            IOqueue.pop_front();
            return nextIOreq;
        } else {
            if (IOqueue.size() == 1) {
                IOreq* ioreq = IOqueue.front();
                IOqueue.pop_front();
                return ioreq;
            }

            IOreq* minDistReq = nullptr;
            int minDist = INT_MAX;
            int offset = 0;
            for (int i = 0; i < IOqueue.size(); i++) {
                if (pos >= IOqueue.at(i)->getTarget()) {
                    int dist = abs(pos - IOqueue.at(i)->getTarget());
                    if (dist < minDist) {
                        minDist = dist;
                        minDistReq = IOqueue.at(i);
                        offset = i;
                    }
                }

            }

            IOqueue.erase(IOqueue.begin() + offset);
            return minDistReq;
        }
    }
}

bool LOOKiosched::isIOqueueEmpty() {
    if (IOqueue.empty()) return true;
    return false;
}

CLOOKiosched::CLOOKiosched() {}

void CLOOKiosched::addIOrequest(IOreq *req) {
    int size = IOqueue.size();
    for (deque<IOreq*>::iterator iter = IOqueue.begin(); iter != IOqueue.end(); iter++) {
        if (req->getTarget() < (*iter)->getTarget()) {
            IOqueue.insert(iter, req);
            break;
        }
    }

    if (IOqueue.size() == size) {
        IOqueue.push_back(req);
    }
}

IOreq* CLOOKiosched::getNextIOrequest(int pos, bool direction) {
    if (IOqueue.back()->getTarget() <= pos) {
        IOreq* nextIOreq = IOqueue.front();
        IOqueue.pop_front();
        return nextIOreq;
    } else {
        if (IOqueue.size() == 1) {
            IOreq* ioreq = IOqueue.front();
            IOqueue.pop_front();
            return ioreq;
        }

        IOreq* minDistReq = nullptr;
        int minDist = INT_MAX;
        int offset = 0;
        for (int i = 0; i < IOqueue.size(); i++) {
            if (pos <= IOqueue.at(i)->getTarget()) {
                int dist = abs(IOqueue.at(i)->getTarget() - pos);
                if (dist < minDist) {
                    minDist = dist;
                    minDistReq = IOqueue.at(i);
                    offset = i;
                }
            }

        }

        IOqueue.erase(IOqueue.begin() + offset);
        return minDistReq;
    }

}

bool CLOOKiosched::isIOqueueEmpty() {
    if (IOqueue.empty()) return true;
    return false;
}

FLOOKiosched::FLOOKiosched() {}

void FLOOKiosched::addIOrequest(IOreq *req) {
    int size = addQ.size();
    for (deque<IOreq*>::iterator iter = addQ.begin(); iter != addQ.end(); iter++) {
        if (req->getTarget() < (*iter)->getTarget()) {
            addQ.insert(iter, req);
            break;
        }
    }

    if (addQ.size() == size) {
        addQ.push_back(req);
    }
}

IOreq* FLOOKiosched::getNextIOrequest(int pos, bool direction) {
    if (activeQ.empty()) {
        //cout << "swap" << endl;
        deque<IOreq*> tmp = activeQ;
        activeQ = addQ;
        addQ = tmp;
    }

    if (direction) { // head is in direction of increment
        // TODO check LOOK and CLOOK
        if (activeQ.back()->getTarget() <= pos) {
            IOreq* nextIOreq = activeQ.back();
            int offset = activeQ.size() - 1;
            for (int i = 0; i < activeQ.size(); i++) {
                if (activeQ.at(i)->getTarget() == nextIOreq->getTarget()) {
                    //cout << "here" << endl;
                    nextIOreq = activeQ.at(i);
                    offset = i;
                    break;
                }
            }

            activeQ.erase(activeQ.begin() + offset);
            return nextIOreq;
        } else {
            if (activeQ.size() == 1) {
                IOreq* ioreq = activeQ.front();
                activeQ.pop_front();
                return ioreq;
            }

            IOreq* minDistReq = nullptr;
            int minDist = INT_MAX;
            int offset = 0;
            for (int i = 0; i < activeQ.size(); i++) {
                if (pos <= activeQ.at(i)->getTarget()) {
                    int dist = abs(activeQ.at(i)->getTarget() - pos);
                    if (dist < minDist) {
                        minDist = dist;
                        minDistReq = activeQ.at(i);
                        offset = i;
                    }
                }
            }

            activeQ.erase(activeQ.begin() + offset);
            return minDistReq;
        }
    } else { // head is in direction of decrement

        if (activeQ.front()->getTarget() >= pos) {
            IOreq* nextIOreq = activeQ.front();
            activeQ.pop_front();
            return nextIOreq;
        } else {
            if (activeQ.size() == 1) {
                IOreq* ioreq = activeQ.front();
                activeQ.pop_front();
                return ioreq;
            }

            IOreq* minDistReq = nullptr;
            int minDist = INT_MAX;
            int offset = 0;
            for (int i = 0; i < activeQ.size(); ++i) {
                if (pos >= activeQ.at(i)->getTarget()) {
                    int dist = abs(pos - activeQ.at(i)->getTarget());
                    if (dist < minDist) {
                        minDist = dist;
                        minDistReq = activeQ.at(i);
                        offset = i;
                    }
                }
            }
            activeQ.erase(activeQ.begin() + offset);
            return minDistReq;
        }
    }
}

bool FLOOKiosched::isIOqueueEmpty() {
    if (activeQ.empty() && addQ.empty()) return true;
    return false;
}