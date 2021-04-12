#pragma once

#include "Romi32U4.h"

class reflectance{
    public:
        bool both(); //true if both are on black
        void setup();
        void set();
    private:
        int black = 900;   
        int blackDetect = black - 200;
        int white = 590;
        int leftSensor = A3;
        int rightSensor = A2;
};