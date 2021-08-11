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
int totalMovement = 0;
bool direction = true; // true: increment, false: decrement

IOscheduler* IOsched;

int main() {
    initIOrequests("/Users/yjeonlee/Desktop/Operating_Systems/Operating-Systems-Labs/lab4_iosched/inputs/input4");

    int timer = 0;
    bool isIOactive = false;
    IOreq* curIOreq = nullptr;

    IOsched = new FLOOKiosched();
    int finishedCnt = 0;

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
            finishedCnt++;

            cout << timer << ":     " << curIOreq->getReqId() << " finish " << timer - curIOreq->getArrivalTime() << endl;
            curIOreq = nullptr;
        }

        // if no IO request active now
        if (!isIOactive) {
            if (isAllIOreqProcessed()) {
                break;
            } else if (!IOsched->isIOqueueEmpty()) {
                curIOreq = IOsched->getNextIOrequest(head, direction);
                curIOreq->start = timer;
                isIOactive = true;

                if (curIOreq->getTarget() > head) {
                    direction = true;
                } else if (curIOreq->getTarget() < head){
                    direction = false;
                } else {
                    continue;
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

    int totalTurnaroundTime = 0;
    int totalWaitTime = 0;
    int maxWaitTime = -1;
    // print results
    for (int i = 0; i < IOrequests.size(); i++) {
        printf("%5d: %5d %5d %5d\n", IOrequests.at(i)->getReqId(), IOrequests.at(i)->getArrivalTime(), IOrequests.at(i)->start, IOrequests.at(i)->end);
        totalTurnaroundTime += (IOrequests.at(i)->end - IOrequests.at(i)->getArrivalTime());
        totalWaitTime += (IOrequests.at(i)->start - IOrequests.at(i)->getArrivalTime());
        if (maxWaitTime < (IOrequests.at(i)->start - IOrequests.at(i)->getArrivalTime())) {
            maxWaitTime = IOrequests.at(i)->start - IOrequests.at(i)->getArrivalTime();
        }
    }

    double avgTurnaround = (double) totalTurnaroundTime / (double) IOrequests.size();
    double avgWaitTime = (double) totalWaitTime / (double) IOrequests.size();
    printf("SUM: %d %d %.2lf %.2lf %d\n",
           timer, totalMovement, avgTurnaround, avgWaitTime, maxWaitTime);

    return 0;
}

bool isAllIOreqProcessed() {
    // TODO change to counter
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
    totalMovement++;
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