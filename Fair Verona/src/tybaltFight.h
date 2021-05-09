#pragma once

#include "commonCode.h"

class tybaltFight{
    public:
        void setup();
        void loop();
    private:
        commonCode c;
    typedef enum{
        IDLE,
        CIRCLE,
        TURNTOMERCOOLIO,
        DRIVETOTAG,
        DRIVETOMERCOOLIO,
        WAITTODIE,
        STOP,
        WAIT,
        TEST
    } State;
    State state;
    State nextState;

    bool enteringState = 1;
    unsigned long timeLast, waitTime, printTime;
    float thetaLast, turnAngle;
    int count;
    float distance, distanceLast;
};