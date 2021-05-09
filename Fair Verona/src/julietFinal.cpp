#include "julietFinal.h"

void julietFinal::setup() {
    c.tapDetectorOn = 1;
    c.setup();
    c.simpleLED.off();
    enteringState = 1;
    state = WAIT;
    nextState = WAITFORCOLLISION;
    waitTime = 2000;
}

void julietFinal::loop() {

     if(millis() - printTime > 500){
        // Serial.print(state);
    }

    c.loop();
    // Serial.println(c.chassis->getDistanceCamera());
    
    switch(state){
    
    case TEST:
        // if (enteringState) {
        //     enteringState = 0;
            // c.chassis->FollowAprilTag(15, 10);
        // }

    break;

    case IDLE:
        if(c.remoteCode == remotePlayPause) state = WAITFORCOLLISION;
            
    break;

    case WAITFORCOLLISION:

        if (c.tapper.CheckTap()) {
            state = WAIT;
            nextState = AWAKE;
            waitTime = 1000;
            enteringState = 1;
        }
            
    break;

    case AWAKE:
        
        c.simpleLED.on();
        state = STOP;

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