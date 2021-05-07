#include "tybaltFight.h"

void tybaltFight::setup() {
    c.tapper.fastSettingsEnable = 0;
    c.setup();
    
    state = WAIT;
    nextState = CIRCLE;
    waitTime = 3000;
}

void tybaltFight::loop(){
    if(millis() - printTime > 500){
        // Serial.print(c.chassis->theta);
        // Serial.print("\n");
    }

    c.loop();

    switch(state) {

        case IDLE:
            if(enteringState){
                enteringState = 0;
            }
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
                nextState = DRIVETOMERCOOLIO;
                waitTime = 1000;
            }
            
        break;

        case DRIVETOMERCOOLIO:
            if(enteringState) {
                enteringState = 0;
            }

            c.chassis->FollowAprilTag(-6, 20);

            if (c.tapper.CheckTap()) {
                enteringState = 1;
                state = WAIT;
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