#ifndef SYSTEMSTATE_H
#define SYSTEMSTATE_H

enum class SystemState {
        PRE_FLIGHT = 1,
        READY,
        DURING_FLIGHT,
        DURING_DESCENT,
        LOW_BATTERY,
        LANDED,
        INVALID = -1
};

#endif // SYSTEMSTATE_H
