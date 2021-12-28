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
        TURNTOSTART,
        LOOKINGFORTAG,
        R2D2,
        TURNTODRIVE,
        DRIVETOBOTTOM,
        LOOKINGFORJULIET,
        FOLLOWJULIET,
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
};