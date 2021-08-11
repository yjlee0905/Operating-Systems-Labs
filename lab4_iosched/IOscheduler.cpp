//
// Created by Yunjeon Lee on 2021/08/05.
//

#include <cstdlib>
#include "IOscheduler.h"

FIFOiosched::FIFOiosched() {}

void FIFOiosched::addIOrequest(IOreq *req) {
    IOreqQ.push_back(req);
}

IOreq* FIFOiosched::getNextIOrequest(int pos, bool direction) {
    if (!IOreqQ.empty()) {
        IOreq* ioreq = IOreqQ.front();
        IOreqQ.pop_front();
        return ioreq;
    }
    return nullptr;
}

bool FIFOiosched::isIOqueueEmpty() {
    if (IOreqQ.empty()) return true;
    return false;
}

SSTFiosched::SSTFiosched() {}

void SSTFiosched::addIOrequest(IOreq *req) {
    IOreqQ.push_back(req);
}

IOreq* SSTFiosched::getNextIOrequest(int pos, bool direction) {
    if (IOreqQ.size() == 1) {
        IOreq* ioreq = IOreqQ.front();
        IOreqQ.pop_front();
        return ioreq;
    }

    IOreq* minIOreq = IOreqQ.at(0);
    int offset = 0;
    for (int i = 1; i < IOreqQ.size(); i++) {
        if (abs(pos - minIOreq->getTarget()) > abs(pos - IOreqQ.at(i)->getTarget())) {
            minIOreq = IOreqQ.at(i);
            offset = i;
        }
    }

    IOreqQ.erase(IOreqQ.begin() + offset);
    return minIOreq;
}

bool SSTFiosched::isIOqueueEmpty() {
    if (IOreqQ.empty()) return true;
    return false;
}

LOOKiosched::LOOKiosched() {}

void LOOKiosched::addIOrequest(IOreq* req) {
    int size = IOreqQ.size();
    for (deque<IOreq*>::iterator iter = IOreqQ.begin(); iter != IOreqQ.end(); iter++) {
        if (req->getTarget() < (*iter)->getTarget()) {
            IOreqQ.insert(iter, req);
            break;
        }
    }

    if (IOreqQ.size() == size) {
        IOreqQ.push_back(req);
    }
}

IOreq* LOOKiosched::getNextIOrequest(int pos, bool direction) {
    if (direction) { // head is in direction of increment
        if (IOreqQ.back()->getTarget() <= pos) {
            IOreq* nextIOreq = IOreqQ.back();
            IOreqQ.pop_back();
            return nextIOreq;
        } else if (IOreqQ.back()->getTarget() > pos){
            if (IOreqQ.size() == 1) {
                IOreq* ioreq = IOreqQ.front();
                IOreqQ.pop_front();
                return ioreq;
            }

            IOreq* minDistReq = nullptr;
            int minDist = INT_MAX;
            int offset = 0;
            for (int i = 0; i < IOreqQ.size(); i++) {
                if (pos <= IOreqQ.at(i)->getTarget()) {
                    int dist = abs(IOreqQ.at(i)->getTarget() - pos);
                    if (dist < minDist) {
                        minDist = dist;
                        minDistReq = IOreqQ.at(i);
                        offset = i;
                    }
                }

            }

//            IOreq* minDistReq = IOreqQ.at(0);
//            int offset = 0;
//            for (int i = 1; i < IOreqQ.size(); i++) {
//                if ((pos <= IOreqQ.at(i)->getTarget()) &&
//                    abs(pos - IOreqQ.at(i)->getTarget()) < abs(pos - minDistReq->getTarget())) {
//                    minDistReq = IOreqQ.at(i);
//                    offset = i;
//                }
//            }

            IOreqQ.erase(IOreqQ.begin() + offset);
            return minDistReq;
        }
    } else { // head is in direction of decrement
        if (IOreqQ.front()->getTarget() >= pos) { // change direction
            IOreq* nextIOreq = IOreqQ.front();
            IOreqQ.pop_front();
            return nextIOreq;
        } else {
            // 더 크면 그 앞에거
            if (IOreqQ.size() == 1) {
                IOreq* ioreq = IOreqQ.front();
                IOreqQ.pop_front();
                return ioreq;
            }

            IOreq* minDistReq = nullptr;
            int minDist = INT_MAX;
            int offset = 0;
            for (int i = 0; i < IOreqQ.size(); i++) {
                if (pos >= IOreqQ.at(i)->getTarget()) {
                    int dist = abs(pos - IOreqQ.at(i)->getTarget());
                    if (dist < minDist) {
                        minDist = dist;
                        minDistReq = IOreqQ.at(i);
                        offset = i;
                    }
                }

            }


//            IOreq* minDistReq = IOreqQ.at(0);
//            int offset = 0;
//            for (int i = 1; i < IOreqQ.size(); i++) {
//                if ((pos >= IOreqQ.at(i)->getTarget()) &&
//                        abs(pos - IOreqQ.at(i)->getTarget()) < abs(pos - minDistReq->getTarget())) {
//                    minDistReq = IOreqQ.at(i);
//                    offset = i;
//                }
//            }

            IOreqQ.erase(IOreqQ.begin() + offset);
            return minDistReq;

//            IOreq* next = IOreqQ.at(0);
//            for (int i = 1; i < IOreqQ.size(); i++) {
//                if (pos <= IOreqQ.at(i)->getTarget()) {
//                    IOreqQ.erase(IOreqQ.begin() + i-1);
//                    return next;
//                }
//                next = IOreqQ.at(i);
//            }
//
//            IOreq* ioreq = IOreqQ.back();
//            IOreqQ.pop_back();
//            return ioreq;

//            for (deque<IOreq*>::iterator iter = IOreqQ.end(); iter != IOreqQ.begin(); iter--) {
//                if (pos >= (*iter)->getTarget()) {
//                    cout << "pos: " << pos << "     target: " << (*iter)->getTarget() << endl;
//                    IOreq* nextIOreq = *iter;
//                    IOreqQ.erase(iter);
//                    return nextIOreq;
//                }
//            }



        }
    }
}

bool LOOKiosched::isIOqueueEmpty() {
    if (IOreqQ.empty()) return true;
    return false;
}

CLOOKiosched::CLOOKiosched() {}

void CLOOKiosched::addIOrequest(IOreq *req) {
    int size = IOreqQ.size();
    for (deque<IOreq*>::iterator iter = IOreqQ.begin(); iter != IOreqQ.end(); iter++) {
        if (req->getTarget() < (*iter)->getTarget()) {
            IOreqQ.insert(iter, req);
            break;
        }
    }

    if (IOreqQ.size() == size) {
        IOreqQ.push_back(req);
    }
}

IOreq* CLOOKiosched::getNextIOrequest(int pos, bool direction) {
    if (IOreqQ.back()->getTarget() <= pos) {
        IOreq* nextIOreq = IOreqQ.front();
        IOreqQ.pop_front();
        return nextIOreq;
    } else if (IOreqQ.back()->getTarget() > pos){
        if (IOreqQ.size() == 1) {
            IOreq* ioreq = IOreqQ.front();
            IOreqQ.pop_front();
            return ioreq;
        }

        IOreq* minDistReq = nullptr;
        int minDist = INT_MAX;
        int offset = 0;
        for (int i = 0; i < IOreqQ.size(); i++) {
            if (pos <= IOreqQ.at(i)->getTarget()) {
                int dist = abs(IOreqQ.at(i)->getTarget() - pos);
                if (dist < minDist) {
                    minDist = dist;
                    minDistReq = IOreqQ.at(i);
                    offset = i;
                }
            }

        }

        IOreqQ.erase(IOreqQ.begin() + offset);
        return minDistReq;
    }

}

bool CLOOKiosched::isIOqueueEmpty() {
    if (IOreqQ.empty()) return true;
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
    if (direction) { // head is in direction of increment
        if (activeQ.empty()) {
            deque<IOreq*> tmp = activeQ;
            activeQ = addQ;
            addQ = tmp;
        }

        if (activeQ.back()->getTarget() <= pos) {
            IOreq* nextIOreq = activeQ.back();
            activeQ.pop_back();
            return nextIOreq;
        } else if (activeQ.back()->getTarget() > pos) {
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
        if (activeQ.empty()) {
            deque<IOreq*> tmp = activeQ;
            activeQ = addQ;
            addQ = tmp;
        }

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