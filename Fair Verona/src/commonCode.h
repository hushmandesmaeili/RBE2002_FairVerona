#pragma once

//class to contain all the code that will be common to all of our programs
#include <Arduino.h>
#include "Romi32U4.h"
#include "Chassis.h"
#include "RemoteConstants.h"
#include "IRdecoder.h"
#include "servo32u4.h"
#include "poison.h"

class commonCode{
    public:
        Poison poison;
        Chassis chassis;
        Romi32U4ButtonA buttonA;
        Romi32U4ButtonB buttonB;
        Romi32U4ButtonC buttonC;

        void setup();
        void loop();

        int16_t remoteCode;
        Poison poison;
    private:
};