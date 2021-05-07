#pragma once

#include "commonCode.h"

class romiFinal{
    public:
        void setup();
        void loop();
    private:
        commonCode c;
        typedef enum{
        IDLE,
        DRIVETOJULIETDISTANCE,
        DUMPPOISON,
        DRIVETOCOLLISION,
        DIE,
        WAIT,
        STOP,
        TEST
        } State;
        State state;
        State nextState;

        unsigned long waitTime, timeLast, printTime;
        bool enteringState = 1;
};