#pragma once

#include "Romi32U4.h"

class reflectance{
    public:
        bool both(); //true if both are on black
        void setup();
    private:
        int black = 700;   
        int blackDetect = black - 200;
        int white = 42;
        int leftSensor = A3;
        int rightSensor = A2;
};