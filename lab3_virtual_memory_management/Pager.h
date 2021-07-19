//
// Created by Yunjeon Lee on 2021/07/19.
//

#ifndef OPERATING_SYSTEMS_LABS_PAGER_H
#define OPERATING_SYSTEMS_LABS_PAGER_H

#include "Header.h"

class Pager {
private:
    int hand;
public:
    Pager();
    virtual Frame* selectVictimFrame() = 0;
};

class FIFOpager : public Pager {
private:
    int hand;
public:
    FIFOpager();
    Frame* selectVictimFrame();
};

#endif //OPERATING_SYSTEMS_LABS_PAGER_H