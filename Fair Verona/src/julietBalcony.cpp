#include "julietBalcony.h"

void julietBalcony::setup(){
    c.setup();
}

typedef enum{
    IDLE,
    APPROACH,
    ONRAMP,
    TOPOFRAMP
} State;
State state = IDLE;

bool enteringState = 1;

void julietBalcony::loop(){
    c.loop();
    switch(state){
        case IDLE:
            //while idling will calibrate when button a is pressed (blocking)
            if(c.buttonA.getSingleDebouncedPress() || c.chassis.IsCalibrating()) {
                c.chassis.GetXAverage();
            }

            if(c.remoteCode == remotePlayPause) state = APPROACH;
        break;

        case APPROACH:
            if(enteringState) {
                c.chassis.setMotorSpeeds(30, 30);
                enteringState = 0;
            }

            if(c.chassis.checkIfOnRamp()) {
                state = ONRAMP;
                enteringState = 1;
            }
        break;

        case ONRAMP:
            if(enteringState){
                c.chassis.startWallFollow();
                enteringState = 0;
            }
            if(!c.chassis.checkIfOnRamp()){
                state = TOPOFRAMP;
                c.chassis.stopWallFollow();
                c.chassis.setMotorSpeeds(0, 0);
                enteringState = 1;
            }
        break;

        case TOPOFRAMP:
            if(enteringState){
                enteringState = 0;
                c.chassis.setMotorSpeeds(-15, 15);
            }

            // if(some pose stuff){
            //     c.chassis.setMotorSpeeds(0, 0);
            // }
        break;
    }
}
