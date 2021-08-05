//
// Created by Yunjeon Lee on 2021/08/05.
//

#include <iostream>
#include <fstream>
#include <vector>
#include "IOreq.h"

using namespace std;

void initIOrequests(string fileName);

vector<IOreq*> IOrequests;

int main() {
    cout << "start lab4" << endl;
    initIOrequests("/Users/yjeonlee/Desktop/Operating_Systems/Operating-Systems-Labs/lab4_iosched/inputs/input0");
    cout << IOrequests.size() << endl;
    for (int i = 0; i < IOrequests.size(); ++i) {
        cout << IOrequests[i]->getReqId() << " " << IOrequests[i]->getTimestamp() << " " << IOrequests[i]->getTarget() << endl;
    }
    return 0;
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