#pragma once

//class to contain all the code that will be common to all of our programs
#include <Arduino.h>
#include "Romi32U4.h"
#include "Chassis.h"



class commonCode{
    public:
        void setup();
        void loop();
        int getRightEffort();
        int getLeftEffort();
    private:
        Chassis chassis;
};