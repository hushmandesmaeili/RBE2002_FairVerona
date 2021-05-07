#pragma once

//class to contain all the code that will be common to all of our programs
#include <Arduino.h>
#include "Romi32U4.h"
#include "Chassis.h"
#include "RemoteConstants.h"
#include "IRdecoder.h"
// #include "LEDManager.h"

class commonCode{
    public:

        Chassis* chassis;
        // Romi32U4ButtonA buttonA;
        Romi32U4ButtonB buttonB;
        Romi32U4ButtonC buttonC;
        // LEDManager ledmanager;

        void setup();
        void loop();

        int16_t remoteCode;
    private:
};