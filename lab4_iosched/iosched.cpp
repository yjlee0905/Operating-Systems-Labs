//
// Created by Yunjeon Lee on 2021/08/05.
//

#include <iostream>
#include <fstream>
#include <queue>
#include "IOreq.h"
#include "IOscheduler.h"

using namespace std;

void moveHead();
bool isAllIOreqProcessed();
void initIOrequests(string fileName);

vector<IOreq*> IOrequests;
int ioReqIdx = 0;
int head = 0;
bool direction = true; // true: increment, false: decrement

IOscheduler* IOsched;

int main() {
//    cout << "start lab4" << endl;
    initIOrequests("/Users/yjeonlee/Desktop/Operating_Systems/Operating-Systems-Labs/lab4_iosched/inputs/input0");

    int timer = 0;
    bool isIOactive = false;
    IOreq* curIOreq = nullptr;

    IOsched = new FIFOiosched();

    // simulation
    while (true) {
        // if a new I/O arrived to the system at this current time
        if ((ioReqIdx != IOrequests.size())
            && (timer == IOrequests.at(ioReqIdx)->getArrivalTime())) { // add request IO-queue
            IOreq* newIOreq = IOrequests.at(ioReqIdx);
            ioReqIdx++;
            IOsched->addIOrequest(newIOreq);

            cout << timer << ":     " << newIOreq->getReqId() << " add " << newIOreq->getTarget() << endl;
        }

        // if an IO is active and completed at this time
        if (curIOreq && (head == curIOreq->getTarget())) {
            curIOreq->end = timer;
            isIOactive = false;

            cout << timer << ":     " << curIOreq->getReqId() << " finish " << timer - curIOreq->getArrivalTime() << endl;
        }

        // if no IO request active now
        if (!isIOactive) {
            if (isAllIOreqProcessed()) {
                break;
            } else if (!IOsched->isIOqueueEmpty()) {
                curIOreq = IOsched->getNextIOrequest();
                curIOreq->start = timer;
                isIOactive = true;

                if (curIOreq->getTarget() > head) {
                    direction = true;
                } else {
                    direction = false;
                }

                cout << timer << ":     " << curIOreq->getReqId() << " issue " << curIOreq->getTarget() << " " << head << endl;

            }
        }

        // if an IO is active
        if (isIOactive) {
            moveHead();
        }
        timer++;
    }

    cout << IOrequests.size() << endl;
    for (int i = 0; i < IOrequests.size(); ++i) {
        cout << IOrequests[i]->getReqId() << " " << IOrequests[i]->getArrivalTime() << " " << IOrequests[i]->getTarget()
        << " " << IOrequests[i]->start << " " << IOrequests[i]->end << endl;
    }

    cout << IOrequests.size() << endl;
    for (int i = 0; i < IOrequests.size(); ++i) {
        cout << IOrequests[i]->getReqId() << " " << IOrequests[i]->getArrivalTime() << " " << IOrequests[i]->getTarget()
        << " " << IOrequests[i]->start << " " << IOrequests[i]->end << endl;
    }

    return 0;
}

bool isAllIOreqProcessed() {
    for (int i = 0; i < IOrequests.size(); ++i) {
        if (IOrequests.at(i)->end == -1) {
            return false;
        }
    }
    return true;
}

void moveHead() {
    if (direction) {
        head++;
    } else {
        head--;
    }
}

void initIOrequests(string fileName) {
    ifstream in;
    in.open(fileName);
    if (!in) {
        cerr << "Cannot open in file!" << endl;
        exit(1);
    }

    string line;
    int cnt = 0;
    while (getline(in, line)) {
        if (line.find("#") == 0) continue;

        // convert string to char*
        char* copiedLine = new char[line.size()+1];
        copy(line.begin(), line.end(), copiedLine);
        copiedLine[line.size()] = '\0';

        int timeStamp;
        int target;

        sscanf(copiedLine, "%d %d", &timeStamp, &target);

        // TODO reqId
        IOreq* ioreq = new IOreq(cnt++, timeStamp, target);
        IOrequests.push_back(ioreq);
    }

    in.close();
}