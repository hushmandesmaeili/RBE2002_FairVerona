#pragma once

#include "commonCode.h"

class romiBalcony{
    public:
        void setup();
        void loop();
    private:
        commonCode c;
        typedef enum{
        IDLE,
        DRIVETOSTART,
        LOOKINGFORTAG,
        EXCITE,
        TURNTODRIVE,
        DRIVETOBOTTOM,
        TURNTOJULIET,
        FOLLOWJULIET,
        STOP,
        WAIT
    } State;
    State state = IDLE;
    State nextState;

    bool enteringState = 1;
    unsigned long timeLast, waitTime;
};