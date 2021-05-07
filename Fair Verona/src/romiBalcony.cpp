#include "romiBalcony.h"

void romiBalcony::setup(){
    c.setup();

    state = IDLE;
    nextState = DRIVETOSTART;
    waitTime = 3000;
}

void romiBalcony::loop(){
    if(millis() - printTime > 100){
        Serial.print(c.chassis->x);
        Serial.print(F("\t"));
        Serial.print(c.chassis->y);
        Serial.print(F("\t"));
        Serial.print(c.chassis->theta);
        Serial.print(F("\t"));
        Serial.print(c.chassis->th_target);
        Serial.print(F("\t"));
        Serial.print(c.chassis->AtTargetPosition());
        Serial.print(F("\t"));
        Serial.print(state);
        Serial.print(F("\n"));
    }

    c.loop();
    
    switch(state){
        case TEST:
            // c.ledmanager.fadeInBool = 1;
        break;
        case IDLE:
            if(enteringState){
                enteringState = 0;
                c.chassis->setMotorSpeeds(0,0);
            }
            // if(c.remoteCode == remote1){
            if(c.buttonB.getSingleDebouncedPress()){
                state = DRIVETOSTART;
                enteringState = 1;
            }
        break;

        case DRIVETOSTART:
            if(enteringState){
                enteringState = 0;
                c.chassis->resetPose();
                c.chassis->setMotorSpeeds(15, 15);
            }
            
            if(c.chassis->x > 100){
                c.chassis->setMotorSpeeds(0, 0);
                state = WAIT;
                nextState = TURNTOSTART;
                waitTime = 2000;
                enteringState = 1;
            }
        break;

        case TURNTOSTART:
            if(enteringState){
                enteringState = 0;
                c.chassis->resetPose();
                c.chassis->setMotorSpeeds(-3, 3);
            }
            
            if(c.chassis->theta > 1.57){
                c.chassis->setMotorSpeeds(0, 0);
                state = LOOKINGFORTAG;
            }
        break;

        case LOOKINGFORTAG:
            // if(enteringState){
            //     enteringState = 0;
            // }
            // Serial.println(c.chassis->DetectAprilTag());
            if(c.chassis->DetectAprilTag() != -1){
                state = R2D2;
                enteringState = 1;
            }
        break;

        case R2D2:
            if(enteringState){
                enteringState = 0;
                c.chassis->resetPose();
                count = 0;
                turnAngle = 0.5;
            }

            if(count < 4){
                if(count % 2){
                    c.chassis->setMotorSpeeds(25, -25);
                    if(c.chassis->theta < -turnAngle) count ++;
                } else {
                    c.chassis->setMotorSpeeds(-25, 25);
                    if(c.chassis->theta > turnAngle) count++;
                }
            } else if (count == 4) {
                c.chassis->setMotorSpeeds(-25, 25);
                if(c.chassis->theta > 0) count ++;
            } else {
                state = TURNTODRIVE;
                c.chassis->setMotorSpeeds(0, 0);
                enteringState = 1;
            }
        break;

        case TURNTODRIVE:
            if(enteringState){
                enteringState = 0;
                c.chassis->setMotorSpeeds(10, -10);
            }

            if(c.chassis->theta < -1.57){
                enteringState = 1;
                c.chassis->setMotorSpeeds(0, 0);
                state = WAIT;
                nextState = DRIVETOBOTTOM;
                waitTime = 1000;
            }
        break;

        case DRIVETOBOTTOM:
            if(enteringState){
                c.chassis->resetPose();
                enteringState = 0;
                c.chassis->SetTargetPosition(65.0, 0.0, 1.57);
            }
            
            c.chassis->MoveToPoint();

            if (c.chassis->AtTargetPosition()) {
                enteringState = 1;
                c.chassis->setMotorSpeeds(0, 0);
                state = LOOKINGFORJULIET;
            }
        break;

        case LOOKINGFORJULIET:
            if(c.chassis->DetectAprilTag() == 1) state = FOLLOWJULIET;
        break;

        case FOLLOWJULIET:
            if(enteringState){
                timeLast = millis();
                enteringState = 0;
            }

            c.chassis->FollowAprilTag(20);

            if(millis() - timeLast > 20000){
                state = STOP;
                enteringState = 1;
            }
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
            c.chassis->setMotorSpeeds(0, 0);
        break;
    }
}