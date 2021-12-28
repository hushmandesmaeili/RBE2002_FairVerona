#pragma once

#include "commonCode.h"

class julietBalcony{
    public:
        void setup();
        void loop();
    private:
        commonCode c;
        typedef enum{
        IDLE,
        APPROACH,
        ONRAMP,
        TOPOFRAMP,
        SPINBACK,
        DOWNRAMP,
        ONFLOOR,
        TURNAWAY,
        DRIVEOFF,
        WAIT,
        STOP,
        TEST
        } State;
        State state;
        State nextState;

        bool enteringState = 1;

        bool goingDown;
        unsigned long timeLast, waitTime, printTime;
        float thetaLast = 0;
        int speed;
};