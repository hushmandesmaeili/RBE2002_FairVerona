#include "julietBalcony.h"

void julietBalcony::setup(){
    c.tapDetectorOn = 0;
    c.setup();
    c.chassis->checkRampEnable = 1;

    // c.chassis->SetTargetPosition(60.0, 0.0);
    state = WAIT;
    nextState = APPROACH;
    waitTime = 3000;
    // pinMode(13, OUTPUT);
}


void julietBalcony::loop(){
    if(millis() - printTime > 100){
        Serial.print(state);
        Serial.print(F("\t"));
        Serial.print(c.chassis->estimatedPitchAng);
        Serial.print(F("\n"));
        // Serial.print(c.chassis->th_target);
        // Serial.print(F("\t"));
        // Serial.print(c.chassis->AtTargetPosition());
        // Serial.print(F("\t"));
    }

    c.loop();

    switch(state){
        case TEST:
            // if(enteringState){
                // c.chassis->wallFollowForwardEnable = 0;
                // c.chassis->wallFollowBackwardEnable = 0;
                // enteringState = 0;
                
            // }
            // c.chassis->FollowAprilTag(20);
            // c.chassis->setMotorSpeeds(-10, 10);
            // Serial.println(c.chassis->getDistance());
        break;
        case IDLE:
            // while idling will calibrate when button a is pressed (blocking)
            if(c.buttonC.getSingleDebouncedPress() || c.chassis->IsCalibrating()) {
                c.chassis->GetXAverage();
            }

            // if(c.remoteCode == remotePlayPause) state = APPROACH;
            if(c.buttonB.getSingleDebouncedPress()) state = APPROACH;
        break;

        case APPROACH:
            if(enteringState) {
                c.chassis->wallFollowForwardEnable = 1;
                enteringState = 0;
            }

            if(c.chassis->checkIfOnRamp()) {
                state = ONRAMP;
                enteringState = 1;
            }
        break;

        case ONRAMP:
            if(enteringState){
                enteringState = 0;
            }

            if(!c.chassis->checkIfOnRamp()){
                c.chassis->wallFollowForwardEnable = 0;
                c.chassis->setMotorSpeeds(0, 0);
                state = WAIT;
                nextState = TOPOFRAMP;
                waitTime = 1000;
                enteringState = 1;
            }
        break;

        case TOPOFRAMP:
            if(enteringState){
                enteringState = 0;
                c.chassis->resetPose();
                c.chassis->setMotorSpeeds(-3, 3);
            }

            if(c.chassis->theta > 1.57){
                c.chassis->setMotorSpeeds(0, 0);
                state = WAIT;
                nextState = SPINBACK;
                waitTime = 2000;
                enteringState = 1;
            }
        break;

        case SPINBACK:
            if(enteringState){
                enteringState = 0;
                c.chassis->resetPose();
                c.chassis->setMotorSpeeds(3, -3);
            }

            if(c.chassis->theta - thetaLast < -1.57){
                c.chassis->setMotorSpeeds(0, 0);
                state = WAIT;
                nextState = DOWNRAMP;
                waitTime = 1000;
                enteringState = 1;
            }
        break;

        case DOWNRAMP:
            if(enteringState){
                enteringState = 0;
                c.chassis->wallFollowBackwardEnable = 1;
                goingDown = 0;
            }

            if(c.chassis->checkIfOnRamp()) goingDown = 1;
            else if (goingDown == 1) { //if we just came off the ramp
                state = ONFLOOR;
                enteringState = 1;
            }
        break;

        case ONFLOOR: //wall extends 20 cm backwards past ramp
            if(enteringState){
                c.chassis->wallFollowBackwardEnable = 0;
                enteringState = 0;
                timeLast = millis();
                c.chassis->setMotorSpeeds(-10, -10);
            }

            if(millis() - timeLast > 4000){
                enteringState = 1;
                state = TURNAWAY;
            }
        break;

        case TURNAWAY:
            if(enteringState){
                enteringState = 0;
                c.chassis->resetPose();
                c.chassis->setMotorSpeeds(3, -3);
            }

            if(c.chassis->theta < -1.57){
                c.chassis->setMotorSpeeds(0, 0);
                state = WAIT;
                nextState = DRIVEOFF;
                waitTime = 1000;
                enteringState = 1;
            }

        break;

        case DRIVEOFF:

            if (enteringState) {
                enteringState = 0;
                c.chassis->resetPose();
                c.chassis->SetTargetPosition(60.0, 30.0, 1.57);
            }

            c.chassis->MoveToPoint();

            if (c.chassis->AtTargetPosition(c.chassis->BUFFER_TARGET_POSE_STD + 2, c.chassis->BUFFER_FINAL_HEADING_STD + 0.25)) {
                c.chassis->setMotorSpeeds(15, 15);
                state = WAIT;
                nextState = STOP;
                waitTime = 8000;
                enteringState = 1;
            }
            
        break;

        case STOP:
            if(enteringState){
                enteringState = 0;
                c.chassis->setMotorSpeeds(0, 0);
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
