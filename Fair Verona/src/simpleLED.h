#pragma once
#include <Romi32U4.h>

class SimpleLED{
    public:
        void setup();
        void on();
        void off();
    private:
        const int LEDPin = 21;  //Also Pin A
};