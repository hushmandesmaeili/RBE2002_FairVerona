#pragma once

#include "commonCode.h"

class julietFinal{
    public:
        void setup();
        void loop();
    private:
        commonCode c;
        typedef enum{
        IDLE,
        WAITFORCOLLISION,
        AWAKE,
        WAIT,
        STOP,
        TEST
        } State;
        State state;
        State nextState;

        unsigned long waitTime, timeLast, printTime;
        bool enteringState = 1;
};