#pragma once
#include <Romi32U4.h>
#include <servo32u4.h>

class Poison {
    public:
        void setup(void);
        void up(void);
        void upish(void);
        void down(void);
        bool done(void);


        int getposition(void);

    private:
        const int servoPotPin = 18;
        const int tolerance = 10;
        int setPoint = A0;
        int UP = 174;
        int DOWN = 285;
        Servo32U4 servo;
};