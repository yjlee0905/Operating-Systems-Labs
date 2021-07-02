//
// Created by Yunjeon Lee on 2021/06/28.
//

#include <iostream>
#include <fstream>
#include "Process.h"
#include "Event.h"

using namespace std;

void initEventQueue(string fileName);

EventQueue evtQueue;

int main() {
    string fileName = "/Users/yjeonlee/Desktop/Operating_Systems/Operating-Systems-Labs/lab2_scheduler/inputs/input6";
    initEventQueue(fileName);
}

void initEventQueue(string fileName) {
    ifstream input;
    input.open(fileName);
    if (!input) {
        cerr << "Cannot open file!" << endl;
        exit(1);
    }

    string line;
    const char delim[] = " \t\r\n\v\f";
    int pid = 0;
    while (getline(input, line)) {
        // convert string to char *
        char *copiedLine = new char[line.size()+1];
        copy(line.begin(), line.end(), copiedLine);
        copiedLine[line.size()] = '\0';
        cout << copiedLine << endl;

        char * ptr = strtok(copiedLine, delim);
        int parsed[4];
        int i=0;
        while (ptr != NULL) {
            cout << ptr  << "     converted: " << atoi(ptr) << endl;
            parsed[i++] = atoi(ptr);
            ptr = strtok(NULL, delim);
        }
        Process *p = new Process(++pid, parsed[0], parsed[1], parsed[2], parsed[3]);
        Event* evt = new Event(p, STATE_CREATED, STATE_READY);
        evtQueue.putEvent(evt);
    }
    cout << evtQueue.eventQ.size() << endl;
    input.close();
}

