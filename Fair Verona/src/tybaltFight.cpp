#include "tybaltFight.h"

void tybaltFight::setup() {
    c.tapper.fastSettingsEnable = 0;
    c.setup();
    
    state = IDLE;
    // nextState = CIRCLE;
    // waitTime = 3000;
}

void tybaltFight::loop(){
    if(millis() - printTime > 500){
        // Serial.print(c.chassis->theta);
        // Serial.print("\n");
    }

    c.loop();

    switch(state) {

        case IDLE:
            if(c.remoteCode == remotePlayPause) state = CIRCLE;
        break;

        case CIRCLE:
            if(enteringState) {
                enteringState = 0;
                c.chassis->driveCircle(18.0, 25.0);
            }

            if (c.chassis->theta > (6 * 3.14)) {
                enteringState = 1;
                c.chassis->setMotorSpeeds(0, 0);
                state = WAIT;
                nextState = TURNTOMERCOOLIO;
                waitTime = 1000;
            }
            
        break;

        case TURNTOMERCOOLIO:
            if(enteringState) {
                enteringState = 0;
                c.chassis->resetPose();
                c.chassis->setMotorSpeeds(-5, 5);
            }

            if(c.chassis->theta > 1.57){
                c.chassis->setMotorSpeeds(0, 0);
                enteringState = 1;
                state = WAIT;
                nextState = DRIVETOTAG;
                waitTime = 1000;
            }
            
        break;

        case DRIVETOTAG:
            c.chassis->setMotorSpeeds(20, 20);

            // state = WAIT;
            // nextState = DRIVETOMERCOOLIO;
            // waitTime = 500;

            if (c.chassis->DetectAprilTag() != -1) {
                enteringState = 1;
                state = DRIVETOMERCOOLIO;
            }
            
        break;

        case DRIVETOMERCOOLIO:
            if(enteringState) {
                timeLast = millis();
                enteringState = 0;
            }

            c.chassis->FollowAprilTag(-6, 25);

            distance = c.chassis->getDistanceCamera();
            if(distance != 33) distanceLast = distance;

            if (distanceLast < 0 && c.tapper.CheckTap()) {
            // if (millis() - timeLast > 500 && c.tapper.CheckTap()) {
                enteringState = 1;
                c.chassis->setMotorSpeeds(0, 0);
                state = WAIT;
                // c.simpleLED.off();
                nextState = WAITTODIE;
                waitTime = 1000;
            }
        break;

        case WAITTODIE:
            if(enteringState) {
                enteringState = 0;
            }
            
            if (c.tapper.CheckTap()) {
                c.simpleLED.off();
                enteringState = 1;
                state = STOP;
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