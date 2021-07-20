//
// Created by Yunjeon Lee on 2021/07/18.
//

#include <iostream>
#include <stdio.h>
#include <fstream>
#include <string>
#include <vector>
#include <deque>
#include "Header.h"
#include "Process.h"
#include "Pager.h"

using namespace std;

void simulation();
Frame* getFrame(Pager* pager);
void initFrameTables(int frameSize, frame_t& frameTable, deque<Frame*>& freeList);
void initProcsAndInstructions(string fileName);
void readRandomNums(string fileName);
int getRandom(int size);

Pager* pager;
frame_t frameTable;
deque<Frame*> freeList;

vector<Process*> procs;
vector<Instruction> instructions;

// TODO check can move to RR algo
vector<int> randomNums; // max : 4611686018427387903(built as 64-bit target), 1073741823(built as 32-bit target)
int rofs = 0;

int main() {
    int pageFrameNum = 32; // will be set through input

    string rFileName = "/Users/yjeonlee/Desktop/Operating_Systems/Operating-Systems-Labs/lab3_virtual_memory_management/inputs/rfile";
    readRandomNums(rFileName);

    string inFileName = "/Users/yjeonlee/Desktop/Operating_Systems/Operating-Systems-Labs/lab3_virtual_memory_management/inputs/in8";
    initProcsAndInstructions(inFileName);
    initFrameTables(pageFrameNum, frameTable, freeList);

    pager = new FIFOpager(pageFrameNum);
    simulation();

    return 0;
}

void simulation() {
    int idx = 0;
    Process* curProc;
    while (idx < instructions.size()) {
        Instruction curInstr = instructions.at(idx);
        cout << idx << ": ==> " << curInstr.operation << " " << curInstr.id << endl;

        // handle special case of "c" and "e" instruction
        if (curInstr.operation == 'c') {
            // set current process
            for (int i = 0; i < procs.size(); i++) {
                if (procs.at(i)->getPID() == curInstr.id) {
                    curProc = procs[curInstr.id];
                    break;
                }
            }
            // TODO change current page table pointer
            idx++;
            continue; // skip below codes
        } else if (curInstr.operation == 'e') {
            idx++;
            continue; // skip below codes
        }

        // now the real instructions of read and write
        PTE* pte = &curProc->pageTable.PTEtable[curInstr.id];
        if (!pte->present) {
            // this in reality generates the page fault exception and now you execute
            // verify this is actually a valid page in a vma if not raise error and next inst

            bool isValidAddr = false;
            if (pte->hole == 1) {
                isValidAddr = true;
            } else {
                for (int i = 0; i < curProc->getVMAs().size(); i++) {
                    if (curProc->getVMAs().at(i).startingVirtualPage <= curInstr.id && curInstr.id <= curProc->getVMAs().at(i).endingVirtualPage) {
                        pte->writeProtected = curProc->getVMAs().at(i).writeProtected;
                        pte->fileMapped = curProc->getVMAs().at(i).fileMapped;
                        pte->hole = 1;
                        isValidAddr = true;
                    }
                }
            }

            if (!isValidAddr) {
                cout << " SEGV" << endl;
                idx++;
                continue;
            }

            Frame* newFrame = getFrame(pager);

            if (newFrame->isVictim) {
                cout << " UNMAP " << newFrame->pid << ":" << newFrame->vpage << endl;

                int originalPid = newFrame->pid;
                int originalVpage = newFrame->vpage;

                PTE* originalProc = &procs.at(originalPid)->pageTable.PTEtable[originalVpage];
                originalProc->present = 0;

                if (originalProc->modified) {
                    if (originalProc->fileMapped) {
                        originalProc->modified = false;
                        cout << " FOUT" << endl;
                    } else {
                        originalProc->modified = false;
                        originalProc->pagedOut = true;
                        cout << " OUT" << endl;
                    }
                }
            }

            if (pte->fileMapped) {
                cout << " FIN" << endl;
            } else {
                if (pte->pagedOut) {
                    cout << " IN" << endl;
                } else {
                    cout << " ZERO" << endl;
                }
            }

            newFrame->pid = curProc->getPID();
            newFrame->vpage = curInstr.id;
            cout << " MAP " << newFrame->frameNum << endl;

            pte->pageFrameNumber = newFrame->frameNum;
            pte->present = 1;



            // figure out if/what to do with old frame if it was mapped
            // see general outline in MM-slides under Lab3 header and writeup below
            // see whether and how to bring in the content of the access page.
        }

        if (curInstr.operation == 'r') {
            pte->referenced = 1;
        } else if (curInstr.operation == 'w') {
            pte->referenced = 1;

            if (pte->writeProtected) {
                cout << " SEGPROT" << endl;
            } else {
                pte->modified = 1;
            }
        }

        // check write protection
        // simulate instruction execution by hardware by updating the R/M PTE bits


        idx++;
    }
}

Frame* getFrame(Pager* pager) {
    if (freeList.size() == 0) {
        return pager->selectVictimFrame(frameTable);
    } else {
        Frame* frame = freeList.front();
        freeList.pop_front();
        return frame;
    }
}

void initFrameTables(int frameSize, frame_t& frameTable, deque<Frame*>& freeList) {
    for (int i = 0; i < frameSize; i++) {
        frameTable.frameTable[i].frameNum = i;
        frameTable.frameTable[i].pid = -1;
        frameTable.frameTable[i].vpage = -1;
        frameTable.frameTable[i].isFree = true;
        frameTable.frameTable[i].isVictim = false;

        Frame* frame = &frameTable.frameTable[i];
        freeList.push_back(frame);
    }
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
                procs.push_back(newProc);
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