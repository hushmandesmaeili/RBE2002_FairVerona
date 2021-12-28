#pragma once

//class to contain all the code that will be common to all of our programs
#include <Arduino.h>
#include "Romi32U4.h"
#include "Chassis.h"
#include "RemoteConstants.h"
#include "IRdecoder.h"
#include "tap_detector.h"
#include "poison.h"
#include "simpleLED.h"
// #include "LEDManager.h"

class commonCode{
    public:

        Chassis* chassis;
        // Romi32U4ButtonA buttonA;
        Romi32U4ButtonB buttonB;
        Romi32U4ButtonC buttonC;
        // LEDManager ledmanager;
        TapDetector tapper;
        Poison poison;
        SimpleLED simpleLED;

        void setup();
        void loop();

        int16_t remoteCode;

        bool tapDetectorOn = 1;
};