//
// Created by Yunjeon Lee on 2021/07/18.
//

#include <iostream>
#include <stdio.h>
#include <fstream>
#include <vector>
#include "Process.h"

using namespace std;

void initProcsAndInstructions(string fileName);
void readRandomNums(string fileName);
int getRandom(int size);

vector<Process> procs;
vector<Instruction> instructions;

// TODO check can move to RR algo
vector<int> randomNums; // max : 4611686018427387903(built as 64-bit target), 1073741823(built as 32-bit target)
int rofs = 0;

int main() {
    string rFileName = "/Users/yjeonlee/Desktop/Operating_Systems/Operating-Systems-Labs/lab3_virtual_memory_management/inputs/rfile";
    readRandomNums(rFileName);

    string inFileName = "/Users/yjeonlee/Desktop/Operating_Systems/Operating-Systems-Labs/lab3_virtual_memory_management/inputs/in2";
    initProcsAndInstructions(inFileName);
    cout << procs.size() << endl;
    for (int i=0; i<procs.size(); i++) {
        //cout << procs.at(i).getPID() << endl;

        for (int j=0; j<procs.at(i).getVMAs().size(); j++) {
            VMA vma = procs.at(i).getVMAs().at(j);
            cout << vma.startingVirtualPage << " " << vma.endingVirtualPage << " " << vma.writeProtected << " " << vma.startingVirtualPage << endl;

        }
    }
    cout << instructions.size() << endl;
    for (int i=0; i<instructions.size(); i++) {
        cout << instructions.at(i).operation << " " << instructions.at(i).id << endl;
    }

    return 0;
}

void initProcsAndInstructions(string fileName) {
    ifstream in;
    in.open(fileName);
    if (!in) {
        cerr << "Cannot open in file!" << endl;
        exit(1);
    }

    string line;
    int procCnt = -1;
    int parsedProcCnt = 0;
    while (getline(in, line)) {
        if (!(line.find("#") == 0)) {
            if (procCnt == -1) {
                procCnt = stoi(line);
            } else if (procCnt != parsedProcCnt) {
                int VMAsCnt = stoi(line);
                vector<VMA> VMAs;

                for (int i=0; i<VMAsCnt; i++) {
                    getline(in, line);
                    // convert string to char *
                    char * copiedLine = new char[line.size() + 1];
                    copy(line.begin(), line.end(), copiedLine);
                    copiedLine[line.size()] = '\0';

                    int startingVirtualPage;
                    int endingVirtualPage;
                    int writeProtected;
                    int fileMapped;

                    sscanf(copiedLine, "%d %d %d %d", &startingVirtualPage, &endingVirtualPage, &writeProtected, &fileMapped);
                    //in >> startingVirtualPage >> endingVirtualPage >> writeProtected >> fileMapped;
                    VMA newVMA;
                    newVMA.startingVirtualPage = startingVirtualPage;
                    newVMA.endingVirtualPage = endingVirtualPage;
                    newVMA.writeProtected = writeProtected;
                    newVMA.fileMapped = fileMapped;

                    VMAs.push_back(newVMA);
                }

                // TODO check *
                Process *newProc = new Process(parsedProcCnt, VMAs);
                procs.push_back(*newProc);

                parsedProcCnt++;
            } else { // read instructions
                Instruction newInstr;
                char * copiedLine = new char[line.size() + 1];
                copy(line.begin(), line.end(), copiedLine);
                copiedLine[line.size()] = '\0';

                sscanf(copiedLine, "%c %d", &newInstr.operation, &newInstr.id);
                instructions.push_back(newInstr);
            }
        }
    }
}

// TODO change to read only when RR algorithm
void readRandomNums(string fileName) {
    ifstream rfile;
    rfile.open(fileName);
    if (!rfile) {
        cerr << "Cannot open rfile!" << endl;
        exit(1);
    }

    string line;
    getline(rfile, line);
    int cnt = stoi(line);

    for (int i = 0; i < cnt; i++) {
        getline(rfile, line);
        randomNums.push_back(stoi(line));
    }

    rfile.close();
}

int getRandom(int size) { // TODO check n should which value?
    int num = randomNums[rofs++] % size;
    if (rofs == randomNums.size()) { // TODO check
        rofs = 0;
    }
    return num;
}