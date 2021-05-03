#include "romiBalcony.h"

void romiBalcony::setup(){
    c.setup();

    state = IDLE;
}

void romiBalcony::loop(){
    if(millis() - printTime > 500){
        Serial.print(state);
        printTime = millis();
        // c.chassis.wallFollowDirection = !c.chassis.wallFollowDirection;
    }

    // c.chassis.FollowAprilTag(20);

    c.loop();
    
    switch(state){
    
        case IDLE:
            if(enteringState){
                enteringState = 0;
                c.chassis.setMotorSpeeds(0,0);
            }
            // if(c.remoteCode == remote1){
            if(c.buttonB.getSingleDebouncedPress()){
                state = DRIVETOSTART;
                enteringState = 1;
            }
        break;

        case DRIVETOSTART:
            // if(enteringState){
            //     enteringState = 0;
            // }
            //need a drive for pose function

            c.chassis.setMotorSpeeds(10, 10);
            waitTime = 3000;
            state = WAIT;
            nextState = LOOKINGFORTAG;
            enteringState = 1;
        break;

        case LOOKINGFORTAG:
            if(enteringState){
                c.chassis.setMotorSpeeds(0, 0);
                enteringState = 0;
            }
            if(c.chassis.DetectAprilTag() == 1){
                state = R2D2;
                enteringState = 1;
            }
        break;

        case R2D2:
            if(enteringState){
                enteringState = 0;
                thetaLast = c.chassis.theta;
                count = 0;
                turnAngle = 10.0;
            }

            if(count < 4){
                if(count % 2){
                    c.chassis.setMotorSpeeds(25, -25);
                    if(c.chassis.theta - thetaLast < -4) count ++;
                } else {
                    c.chassis.setMotorSpeeds(-25, 25);
                    if(c.chassis.theta - thetaLast > 4) count++;
                }
            } else if (count == 4) {
                c.chassis.setMotorSpeeds(-25, 25);
                if(c.chassis.theta - thetaLast > 0) count ++;
            } else {
                state = TURNTODRIVE;
                c.chassis.setMotorSpeeds(0, 0);
                enteringState = 1;
            }
        break;

        case TURNTODRIVE:
            if(enteringState){
                enteringState = 0;
                thetaLast = c.chassis.theta;
                c.chassis.setMotorSpeeds(15, -15);
            }

            if(c.chassis.theta - thetaLast < -25){
                enteringState = 1;
                state = DRIVETOBOTTOM;
            }
        break;

        case DRIVETOBOTTOM:
            c.chassis.setMotorSpeeds(15, 15);
            state = WAIT;
            nextState = STOP;
            waitTime = 2000;
        break;

        case WAIT:
            if(enteringState){
                enteringState = 0;
                timeLast = millis();
            }
            if(millis() - timeLast > waitTime){
                state = nextState;
                enteringState  = 1;
            }
        break;

        case STOP:
            c.chassis.setMotorSpeeds(0, 0);
        break;
    }
}