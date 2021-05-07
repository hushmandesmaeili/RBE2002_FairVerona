#include "romiFight.h"

void romiFight::setup() {
    c.setup();
    state = WAIT;
    nextState = DRIVETOTAG;
    waitTime = 3000;
}

void romiFight::loop(){
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

        case DRIVETOTAG:
            if(enteringState) {
                enteringState = 0;
                c.chassis->setMotorSpeeds(20, 20);
            }

            if (c.chassis->DetectAprilTag() != -1) {
                enteringState = 1;;
                state = DRIVETOTYBALT;
            }
            
        break;

        case DRIVETOTYBALT:
            c.chassis->FollowAprilTag(0, 20);

            if (c.tapper.CheckTap()) {
                enteringState = 1;
                c.chassis->setMotorSpeeds(0, 0);
                state = WAIT;
                nextState = BACKAWAY;
                waitTime = 1000;
            }
            
        break;

        case BACKAWAY:
            if(enteringState) {
                enteringState = 0;
                c.chassis->setMotorSpeeds(-10, -10);
            }

            if (c.chassis->x < (-5.0)) {
                enteringState = 1;
                c.chassis->setMotorSpeeds(0, 0);
                state = TURNTOLEAVE;
            }
            
        break;
        
        case TURNTOLEAVE:
            if(enteringState) {
                enteringState = 0;
                c.chassis->setMotorSpeeds(-10, 10);
            }

            if (c.chassis->theta > (3.14 / 3)) {
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