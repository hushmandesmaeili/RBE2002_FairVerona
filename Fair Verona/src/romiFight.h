#pragma once

#include "commonCode.h"

class romiFight{
    public:
        void setup();
        void loop();
    private:
        commonCode c;
        typedef enum{
        IDLE,
        DRIVETOTAG,
        DRIVETOTYBALT,
        BACKAWAY,
        TURNTOLEAVE,
        EXITSCENE,
        STOP,
        WAIT,
        TEST
        } State;
        State state;
        State nextState;

        bool enteringState = 1;
        unsigned long timeLast, waitTime, printTime;
        float distance, distanceLast;
};