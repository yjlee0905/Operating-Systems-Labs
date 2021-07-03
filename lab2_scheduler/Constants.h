//
// Created by Yunjeon Lee on 2021/07/03.
//

#ifndef OPERATING_SYSTEMS_LABS_CONSTANTS_H
#define OPERATING_SYSTEMS_LABS_CONSTANTS_H

typedef enum {
    STATE_CREATED,
    STATE_READY,
    STATE_RUNNING,
    STATE_BLOCKED,
    STATE_DONE
} process_state_t;

typedef enum {
    TRANS_TO_READY,
    TRANS_TO_RUN,
    TRANS_TO_BLOCK,
    TRANS_TO_PREEMPT
} trans;

inline const char* processStateToString(process_state_t state) {
    switch (state) {
        case STATE_CREATED: return "CREATED";
        case STATE_READY: return "READY";
        case STATE_RUNNING: return "RUNNG";
        case STATE_BLOCKED: return "BLOCK";
        case STATE_DONE: return "Done";
    }
}

#endif //OPERATING_SYSTEMS_LABS_CONSTANTS_H