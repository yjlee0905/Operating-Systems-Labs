//
// Created by Yunjeon Lee on 2021/07/03.
//

#ifndef OPERATING_SYSTEMS_LABS_CONSTANTS_H
#define OPERATING_SYSTEMS_LABS_CONSTANTS_H

typedef enum {
    STATE_CREATED,
    STATE_READY,
    STATE_RUNNING,
    STATE_BLOCKED
} process_state_t;

typedef enum {
    TRANS_TO_READY,
    TRANS_TO_RUN,
    TRANS_TO_BLOCK,
    TRANS_TO_PREEMPT
} trans;

#endif //OPERATING_SYSTEMS_LABS_CONSTANTS_H
