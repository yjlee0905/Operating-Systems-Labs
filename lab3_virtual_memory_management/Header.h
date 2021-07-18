//
// Created by Yunjeon Lee on 2021/07/18.
//

#ifndef OPERATING_SYSTEMS_LABS_HEADER_H
#define OPERATING_SYSTEMS_LABS_HEADER_H

struct VMA {
    int startingVirtualPage;
    int endingVirtualPage;
    int writeProtected; // TODO check change to bool? (0/1)
    int fileMapped;
};

struct Instruction {
    char operation;
    int id; // r,w : id, c, e: procid
};

#endif //OPERATING_SYSTEMS_LABS_HEADER_H
