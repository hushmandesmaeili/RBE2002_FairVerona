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
        WAIT,
        SPINBACK,
        DOWNRAMP,
        ONFLOOR,
        DRIVEOFF,
        STOP,
        TEST
    } State;
    State state = TEST;
    State nextState;

    bool enteringState = 1;

    bool goingDown;
    unsigned long timeLast, waitTime;
    float thetaLast = 0;
    int speed;
};