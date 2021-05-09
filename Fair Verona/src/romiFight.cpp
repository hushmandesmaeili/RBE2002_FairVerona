#include "romiFight.h"

void romiFight::setup() {
    c.setup();
    state = IDLE;
    // nextState = DRIVETOTAG;
    // waitTime = 14000;
}

void romiFight::loop(){
    if(millis() - printTime > 500){
        distance = c.chassis->getDistanceCamera();
        if(distance != 33.00){
            Serial.print(distance);
            Serial.println();
        }
    }

    c.loop();

    switch(state) {

        case TEST:
        // distance = c.chassis->getDistanceCamera();
        // if(distance != 33.00){
        //     Serial.print(distance);
        //     Serial.println();
        // }
            // c.chassis->FollowAprilTag(-6, 25);
        break;

        case IDLE:
            if(c.remoteCode == remotePlayPause) {
                state = WAIT;
                nextState = DRIVETOTAG;
                waitTime = 20000;
                // waitTime = 3000;
                c.poison.down();
            }
        break;

        case DRIVETOTAG:
            if(enteringState) {
                enteringState = 0;
                c.chassis->setMotorSpeeds(20, 20);
            }

            if (c.chassis->DetectAprilTag() != -1) {
                enteringState = 1;
                state = DRIVETOTYBALT;
                // state = WAIT;
                // nextState = DRIVETOTYBALT;
                // waitTime = 750;
            }
            
        break;

        case DRIVETOTYBALT:
            if(enteringState) {
                timeLast = millis();
                enteringState = 0;
            }

            c.chassis->FollowAprilTag(-9, 25);

            distance = c.chassis->getDistanceCamera();
            if(distance != 33) distanceLast = distance;

            if (distanceLast < 0 && c.tapper.CheckTap()) {
            // if (millis() - timeLast > 750 && c.tapper.CheckTap()) {
                enteringState = 1;
                c.chassis->setMotorSpeeds(0, 0);
                state = WAIT;
                // c.simpleLED.off();
                nextState = BACKAWAY;
                waitTime = 1000;
            }
            
        break;

        case BACKAWAY:
            if(enteringState) {
                enteringState = 0;
                c.chassis->resetPose();
                c.chassis->setMotorSpeeds(-10, -10);
            }

            if (c.chassis->x < (-5.0)) {
                enteringState = 1;
                c.chassis->setMotorSpeeds(0, 0);
                // c.simpleLED.on();
                state = TURNTOLEAVE;
            }
            
        break;
        
        case TURNTOLEAVE:
            if(enteringState) {
                enteringState = 0;
                c.chassis->setMotorSpeeds(-10, 10);
            }

            if (c.chassis->theta > (3.14 / 2)) {
                enteringState = 1;
                c.chassis->setMotorSpeeds(0, 0);
                state = EXITSCENE;
            }
            
        break;
        
        case EXITSCENE:
            if(enteringState){
                enteringState = 0;
                c.chassis->resetPose();
                c.chassis->SetTargetPosition(100.0, 10.0, (3.14 / 6));
            }
            
            c.chassis->MoveToPoint();

            if(c.chassis->AtTargetPosition()){
                state = STOP;
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
                nextState = STOP;
            }
        break;
    }
}