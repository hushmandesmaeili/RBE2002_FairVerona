#include "romiBalcony.h"
#include "servo32u4.h"
void romiBalcony::setup(){
    c.setup();

    state = TEST;
}

void romiBalcony::loop(){
    if(millis() - printTime > 500){
        Serial.print(c.chassis.DetectAprilTag());
        printTime = millis();
        // c.chassis.wallFollowDirection = !c.chassis.wallFollowDirection;
    }

    // c.chassis.FollowAprilTag(20);

    c.loop();
    


    switch(state){
        default:
            break;
        case TEST:
            if( c.buttonA.getSingleDebouncedPress()){
                enteringState = 0; 
                c.poison.setup();
                c.poison.up();
                Serial.begin(115200); 
                Serial.print(c.poison.getposition());
                Serial.print("\n");
                c.poison.down();

            }



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

            c.chassis.setMotorSpeeds(5, 5);
            waitTime = 1000;
            state = WAIT;
            nextState = LOOKINGFORTAG;
            enteringState = 1;
        break;

        case LOOKINGFORTAG:
            if(c.chassis.DetectAprilTag() == 16){
                state = R2D2;
            }
        break;

        case WAIT:
            if(enteringState){
                enteringState = 0;
                timeLast = millis();
            }
            if(millis() - timeLast > waitTime){
                state = nextState;
                enteringState  = 0;
            }
        break;
    }
}