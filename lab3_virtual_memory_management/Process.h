//
// Created by Yunjeon Lee on 2021/07/18.
//

#ifndef OPERATING_SYSTEMS_LABS_PROCESS_H
#define OPERATING_SYSTEMS_LABS_PROCESS_H

#include <vector>
#include "Header.h"

using namespace std;

class Process {
private:
    int pid; // TODO check if necessary
    vector<VMA> VMAs;

public:
    Process(int pid, vector<VMA> VMAs);
    int getPID();
    vector<VMA> getVMAs();
};

#endif //OPERATING_SYSTEMS_LABS_PROCESS_H
