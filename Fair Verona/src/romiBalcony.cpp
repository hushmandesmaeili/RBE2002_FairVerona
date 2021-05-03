#include "romiBalcony.h"

void romiBalcony::setup(){
    c.setup();
}

void romiBalcony::loop(){
    c.loop();
    
    switch(state){
    
        case IDLE:
            if(enteringState){
                enteringState = 0;
                c.chassis.setMotorSpeeds(0,0);
            }
            if(c.remoteCode == remote1){
                state = DRIVETOSTART;
                enteringState = 1;
            }
        break;

        case DRIVETOSTART:
            if(enteringState){
                enteringState = 0;
                c.chassis.setMotorSpeeds(5, 5);
                timeLast = millis();
                waitTime = 1000;
            }
            //need a drive for pose counts function
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