#pragma once

#include "Romi32U4Motors.h"

class Chassis{
    public:
        void setup();
        void loop();
        void setMotorEfforts();
        void setMotorEfforts(int lEffort, int rEffort);
        int getLeftEffort();
        int getRightEffort();
    private:
        Romi32U4Motors motors;
        int leftEffort = 0;
        int rightEffort = 0;
};