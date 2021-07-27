//
// Created by Yunjeon Lee on 2021/07/18.
//

#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <fstream>
#include <string>
#include <vector>
#include <deque>
#include "Header.h"
#include "Process.h"
#include "Pager.h"

using namespace std;

void simulation(bool isO);
Frame* getFrame(Pager* pager);
void printStatistics(bool isP, bool isF, bool isS, int pageFrameNum);
void initFrameTables(int frameSize, frame_t& frameTable, deque<Frame*>& freeList);
void initProcsAndInstructions(string fileName);
vector<int> readRandomNums(string fileName);

Pager* pager;
frame_t frameTable;
deque<Frame*> freeList;
bool isVictim = false;

vector<Process*> procs;
vector<Instruction> instructions;

// summary
unsigned long instCount = 0;
unsigned long long instCount_RW = 0;
unsigned long ctxSwitches = 0;
unsigned long processExits = 0;

int main(int argc, char* argv[]) {
    int c;
    int pageFrameNum; // will be set through input
    char pagerType;

    bool isO = false;
    bool isP = false;
    bool isF = false;
    bool isS = false;

    while ((c = getopt(argc, argv, "f:a:o:")) != -1) {
        switch (c) {
            case 'f': // frameNum
                sscanf(optarg, "%d", &pageFrameNum);
                break;
            case 'a': // algorithm
                sscanf(optarg, "%c", &pagerType);
                break;
            case 'o': // options
                for (; *optarg != '\0'; optarg++) {
                    switch (*optarg) {
                        case 'O':
                            isO = true;
                            break;
                        case 'P':
                            isP = true;
                            break;
                        case 'F':
                            isF = true;
                            break;
                        case 'S':
                            isS = true;
                            break;
                        case 'x':
                            // not implemented
                            break;
                        case 'y':
                            // not implemented
                            break;
                        case 'f':
                            // not implemented
                            break;
                        case 'a':
                            // not implemented
                            break;
                        default:
                            cout << c << " is unsupported option in this program." << endl;
                            exit(1);
                    }
                }
                break;
            default:
                cout << c << " is unsupported option in this program." << endl;
                break;
        }
    }

    string inFileName = argv[optind];
    string rFileName = argv[optind + 1];

    switch (pagerType) {
        case 'f':
            pager = new FIFOpager(pageFrameNum);
            break;
        case 'r':
            pager = new RandomPager(pageFrameNum, readRandomNums(rFileName));
            break;
        case 'c':
            pager = new ClockPager(pageFrameNum, frameTable);
            break;
        case 'e':
            pager = new NRUpager(pageFrameNum);
            break;
        case 'a':
            pager = new AgingPager(pageFrameNum);
            break;
        case 'w':
            pager = new WorkingSetPager(pageFrameNum);
            break;
        default:
            cout << pagerType << " is unsupported Pager in this program." << endl;
            exit(1);
    }

    initProcsAndInstructions(inFileName);
    initFrameTables(pageFrameNum, frameTable, freeList);
    simulation(isO);
    printStatistics(isP, isF, isS, pageFrameNum);

    return 0;
}

void simulation(bool isO) {
    int idx = 0;
    Process* curProc;
    while (idx < instructions.size()) {
        Instruction curInstr = instructions.at(idx);
        if (isO) {
            cout << idx << ": ==> " << curInstr.operation << " " << curInstr.id << endl;
        }
        pager->incrementTimer();

        // handle special case of "c" and "e" instruction
        if (curInstr.operation == 'c') {
            // change current process
            for (int i = 0; i < procs.size(); i++) {
                if (procs.at(i)->getPID() == curInstr.id) {
                    curProc = procs.at(i);
                    break;
                }
            }
            ctxSwitches++;
        } else if (curInstr.operation == 'e') {
            cout << "EXIT current process " << curInstr.id << endl;
            processExits++;

            for (int i = 0; i < NUM_OF_PAGES; i++) {
                PTE* entry = &curProc->pageTable.PTEtable[i];
                if (entry->present) {
                    Frame* curFrame = &frameTable.frameTable[entry->pageFrameNumber];
                    if (isO) {
                        cout << " UNMAP " << curFrame->pid << ":" << curFrame->vpage << endl;
                    }
                    curProc->unmaps++;

                    curFrame->pid = -1;
                    curFrame->vpage = -1;
                    curFrame->isFree = true;
                    curFrame->age = 0;
                    curFrame->timeOfLastUse = 0;

                    if (entry->modified && entry->fileMapped) {
                        if (isO) {
                            cout << " FOUT" << endl;
                        }
                        curProc->fouts++;
                    }

                    freeList.push_back(curFrame);
                }

                entry->present = 0;
                entry->referenced = 0;
                entry->modified = 0;
                entry->writeProtected = 0;
                entry->pagedOut = 0;
                entry->pageFrameNumber = -1;
                entry->fileMapped = 0;

            }
        } else if (curInstr.operation == 'r' || curInstr.operation == 'w') {
            // now the real instructions of read and write
            PTE* pte = &curProc->pageTable.PTEtable[curInstr.id];
            if (!pte->present) {
                // this in reality generates the page fault exception and now you execute
                // verify this is actually a valid page in a vma if not raise error and next inst

                if (pte->checkedHole == 0) {
                    for (int i = 0; i < curProc->getVMAs().size(); i++) {
                        if (curProc->getVMAs().at(i).startingVirtualPage <= curInstr.id && curInstr.id <= curProc->getVMAs().at(i).endingVirtualPage) {
                            pte->writeProtected = curProc->getVMAs().at(i).writeProtected;
                            pte->fileMapped = curProc->getVMAs().at(i).fileMapped;
                            pte->notHole = 1;
                            pte->checkedHole = 1;
                            break;
                        }
                    }
                }

                if (pte->notHole == 0) {
                    if (isO) {
                        cout << " SEGV" << endl;
                    }
                    curProc->segv++;
                    idx++;
                    continue;
                }

                Frame* newFrame = getFrame(pager);
                pte->pageFrameNumber = newFrame->frameNum;
                pte->present = 1;

                // figure out if/what to do with old frame if it was mapped
                // see general outline in MM-slides under Lab3 header and writeup below
                // see whether and how to bring in the content of the access page.

                if (isVictim) {
                    if (isO) {
                        cout << " UNMAP " << newFrame->pid << ":" << newFrame->vpage << endl;
                    }
                    procs.at(newFrame->pid)->unmaps++;

                    int originalPid = newFrame->pid;
                    int originalVpage = newFrame->vpage;

                    PTE* originalProc = &procs.at(originalPid)->pageTable.PTEtable[originalVpage];
                    originalProc->present = 0;

                    if (originalProc->modified) {
                        if (originalProc->fileMapped) {
                            if (isO) {
                                cout << " FOUT" << endl;
                            }
                            procs.at(originalPid)->fouts++;

                            originalProc->modified = false;
                        } else {
                            if (isO) {
                                cout << " OUT" << endl;
                            }
                            procs.at(originalPid)->outs++;

                            originalProc->modified = false;
                            originalProc->pagedOut = true;
                        }
                    }
                }

                if (pte->pagedOut) {
                    if (isO) {
                        cout << " IN" << endl;
                    }
                    curProc->ins++;
                } else if (pte->fileMapped) {
                    if (isO) {
                        cout << " FIN" << endl;
                    }
                    curProc->fins++;
                } else {
                    if (isO) {
                        cout << " ZERO" << endl;
                    }
                    curProc->zeros++;
                }

                if (isO) {
                    cout << " MAP " << newFrame->frameNum << endl;
                }
                curProc->maps++;

                newFrame->pid = curProc->getPID();
                newFrame->vpage = curInstr.id;
                newFrame->age = 0;
                newFrame->timeOfLastUse = 0;

            }

            pte->referenced = 1;

            // check write protection
            if (curInstr.operation == 'w') {
                if (pte->writeProtected) {
                    if (isO) {
                        cout << " SEGPROT" << endl;
                    }
                    curProc->segprot++;
                } else {
                    pte->modified = 1;
                }
            }
            // simulate instruction execution by hardware by updating the R/M PTE bits
        }

        // for debug
        //printStatistics(true, true, false, 16);
        idx++;
    }
}

Frame* getFrame(Pager* pager) {
    if (freeList.size() == 0) {
        isVictim = true;
        return pager->selectVictimFrame(frameTable, procs);
    } else {
        Frame* frame = freeList.front();
        freeList.pop_front();
        isVictim = false;
        return frame;
    }
}

void printStatistics(bool isP, bool isF, bool isS, int pageFrameNum) {
    // page table
    if (isP) {
        for (int i = 0; i < procs.size(); i++) {
            cout << "PT[" << i << "]: " ;
            for (int j = 0; j < NUM_OF_PAGES; j++) {
                if (procs.at(i)->pageTable.PTEtable[j].present == 1) {
                    cout << j << ":";

                    if (procs.at(i)->pageTable.PTEtable[j].referenced == 1) {
                        cout << "R";
                    } else {
                        cout << "-";
                    }

                    if (procs.at(i)->pageTable.PTEtable[j].modified == 1) {
                        cout << "M";
                    } else {
                        cout << "-";
                    }

                    if (procs.at(i)->pageTable.PTEtable[j].pagedOut == 1) {
                        cout << "S";
                    } else {
                        cout << "-";
                    }

                    cout << " ";

                } else {
                    if (procs.at(i)->pageTable.PTEtable[j].pagedOut == 1) {
                        cout << "# ";
                    } else {
                        cout << "* ";
                    }
                }
            }
            cout << endl;
        }
    }

    // frame table
    if (isF) {
        cout << "FT: ";
        for (int i = 0; i < pageFrameNum; i++) {
            if (frameTable.frameTable[i].pid == -1) {
                cout << "* ";
            } else {
                cout << frameTable.frameTable[i].pid << ":" << frameTable.frameTable[i].vpage << " ";
            }
        }
        cout << endl;
    }

    if (isS) {
        unsigned long long totalCost = 0;

        //per process output
        for (int i = 0; i < procs.size(); i++) {
            printf("PROC[%d]: U=%lu M=%lu I=%lu O=%lu FI=%lu FO=%lu Z=%lu SV=%lu SP=%lu\n",
                   procs.at(i)->getPID(),
                   procs.at(i)->unmaps, procs.at(i)->maps, procs.at(i)->ins, procs.at(i)->outs,
                   procs.at(i)->fins, procs.at(i)->fouts, procs.at(i)->zeros, procs.at(i)->segv, procs.at(i)->segprot);
            totalCost += COST_MAPS * procs.at(i)->maps + COST_UNMAPS * procs.at(i)->unmaps + COST_INS * procs.at(i)->ins + COST_OUTS * procs.at(i)->outs
                         + COST_FINS * procs.at(i)->fins + COST_FOUTS * procs.at(i)->fouts + COST_ZEROS * procs.at(i)->zeros + COST_SEGV * procs.at(i)->segv + COST_SEGPROT * procs.at(i)->segprot;
        }

        totalCost += COST_RW_INSTR * instCount_RW + COST_CTX_SWITCHES * ctxSwitches + COST_PROC_EXITS * processExits;

        // summary output
        printf("TOTALCOST %lu %lu %lu %llu %lu\n",
               instCount, ctxSwitches, processExits, totalCost, sizeof(PTE));

    }
}

void initFrameTables(int frameSize, frame_t& frameTable, deque<Frame*>& freeList) {
    // TODO check frame table also should be initialized to 0? > deduction?
    for (int i = 0; i < frameSize; i++) {
        frameTable.frameTable[i].frameNum = i;
        frameTable.frameTable[i].pid = -1;
        frameTable.frameTable[i].vpage = -1;
        frameTable.frameTable[i].isFree = true;
        frameTable.frameTable[i].age = 0;
        frameTable.frameTable[i].timeOfLastUse = 0; // TODO -1?

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
                    VMA newVMA;
                    newVMA.startingVirtualPage = startingVirtualPage;
                    newVMA.endingVirtualPage = endingVirtualPage;
                    newVMA.writeProtected = writeProtected;
                    newVMA.fileMapped = fileMapped;

                    VMAs.push_back(newVMA);
                }

                Process *newProc = new Process(parsedProcCnt, VMAs);
                procs.push_back(newProc);
                parsedProcCnt++;
            } else { // read instructions
                Instruction newInstr;
                char * copiedLine = new char[line.size() + 1];
                copy(line.begin(), line.end(), copiedLine);
                copiedLine[line.size()] = '\0';

                sscanf(copiedLine, "%c %d", &newInstr.operation, &newInstr.id);
                instCount++;
                if (newInstr.operation != 'c' && newInstr.operation != 'e') {
                    instCount_RW++;
                }
                instructions.push_back(newInstr);
            }
        }
    }
}

vector<int> readRandomNums(string fileName) {
    vector<int> randomNums; // max : 4611686018427387903(built as 64-bit target), 1073741823(built as 32-bit target)

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

    return randomNums;
}