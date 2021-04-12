#pragma once

//class to contain all the code that will be common to all of our programs
#include <Arduino.h>
#include "Romi32U4.h"
#include "Chassis.h"



class commonCode{
    public:
        Chassis chassis;

        void setup();
        void loop();
    private:
};