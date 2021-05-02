#include "julietBalcony.h"

void julietBalcony::setup(){
    c.setup();
    c.chassis.checkRampEnable = 1;
}

typedef enum{
    IDLE,
    APPROACH,
    ONRAMP,
    TOPOFRAMP,
    WAIT,
    SPINBACK,
    DOWNRAMP,
    ONFLOOR,
    DRIVEOFF,
    STOP
} State;
State state = IDLE;
State nextState;

bool enteringState = 1;

bool goingDown;
unsigned long timeLast, waitTime;
float thetaLast = 0;
int speed;

void julietBalcony::loop(){
    if(millis() - timeLast < 200){
        Serial.println(c.chassis.theta);
        timeLast = millis();
    }

    c.loop();

    switch(state){
        case IDLE:
            //while idling will calibrate when button a is pressed (blocking)
            // if(c.buttonC.getSingleDebouncedPress() || c.chassis.IsCalibrating()) {
            //     c.chassis.GetXAverage();
            // }

            // if(c.remoteCode == remotePlayPause) state = APPROACH;
            if(c.buttonB.getSingleDebouncedPress()) state = APPROACH;
        break;

        case APPROACH:
            if(enteringState) {
                c.chassis.setMotorSpeeds(10, 10);
                enteringState = 0;
            }

            if(c.chassis.checkIfOnRamp()) {
                state = ONRAMP;
                enteringState = 1;
            }
        break;

        case ONRAMP:
            if(enteringState){
                c.chassis.wallFollowEnable = 1;
                c.chassis.wallFollowDirection = 1;
                enteringState = 0;
            }
            if(!c.chassis.checkIfOnRamp()){
                state = WAIT;
                nextState = TOPOFRAMP;
                waitTime = 1000;
                c.chassis.wallFollowEnable = 0;
                c.chassis.setMotorSpeeds(0, 0);
                enteringState = 1;
            }
        break;

        case TOPOFRAMP:
            if(enteringState){
                enteringState = 0;
                thetaLast = c.chassis.theta;
                c.chassis.setMotorSpeeds(-3, 3);
            }

            if(c.chassis.theta - thetaLast > 20){
                c.chassis.setMotorSpeeds(0, 0);
                state = WAIT;
                nextState = SPINBACK;
                waitTime = 2000;
                enteringState = 1;
            }
        break;

        case SPINBACK:
            if(enteringState){
                enteringState = 0;
                thetaLast = c.chassis.theta;
                c.chassis.setMotorSpeeds(3, -3);
            }

            if(c.chassis.theta - thetaLast < -20){
                c.chassis.setMotorSpeeds(0, 0);
                state = WAIT;
                nextState = DOWNRAMP;
                waitTime = 1000;
                enteringState = 1;
            }
        break;

        case DOWNRAMP:
            if(enteringState){
                enteringState = 0;
                c.chassis.wallFollowEnable = 1;
                c.chassis.wallFollowDirection = 0;
                goingDown = 0;
            }

            if(c.chassis.checkIfOnRamp()) goingDown = 1;
            else if (goingDown == 1) { //if we just came off the ramp
                state = ONFLOOR;
                enteringState = 1;
            }
        break;

        case ONFLOOR:
            if(enteringState){
                enteringState = 0;
                thetaLast = c.chassis.theta;
                c.chassis.setMotorSpeeds(-3, 3);
            }

            if(c.chassis.theta - thetaLast > 20){
                c.chassis.setMotorSpeeds(0, 0);
                state = WAIT;
                nextState = DRIVEOFF;
                waitTime = 1000;
                enteringState = 1;
            }

        break;

        case DRIVEOFF:
            c.chassis.setMotorSpeeds(10, 10);
            state = WAIT;
            nextState = STOP;
            waitTime = 3000;
        break;

        case STOP:
            if(enteringState){
                enteringState = 0;
                c.chassis.setMotorSpeeds(0, 0);
            }
        break;

        case WAIT:
            if(enteringState){
                enteringState = 0;
                timeLast = millis();
            }

            if(millis() - timeLast > waitTime){
                state = nextState;
                enteringState = 1;
                waitTime = 0;
            }
        break;
    }
}
