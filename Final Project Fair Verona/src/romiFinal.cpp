#include "romiFinal.h"

void romiFinal::setup(){
    c.tapDetectorOn = 1;
    c.setup();
    enteringState = 1;
    state = IDLE;
    //TURN LED ON
}

void romiFinal::loop() {
    if(millis() - printTime > 500){
        // Serial.print(state);
        Serial.print(c.chassis->getDistanceCamera());
        Serial.print("\n");
    }

    c.loop();
    // Serial.println(c.chassis->getDistanceCamera());
    
    switch(state){
    
    case TEST:
        // if (enteringState) {
            // enteringState = 0;
            // c.chassis->FollowAprilTag(15, 10);
        // }

        // c.chassis->FollowAprilTag(15, 15);

        // if (c.tapper.CheckTap()) {
        //     enteringState = 1;
        //     state = STOP;
        // }

    break;

    case IDLE:

        if(c.remoteCode == remotePlayPause) state = DRIVETOJULIETDISTANCE;
            
    break;

    case DRIVETOJULIETDISTANCE:

        if (enteringState) {
            enteringState = 0;
            c.chassis->setMotorSpeeds(10, 10);
        }
        
        if (c.chassis->getDistanceCamera() <= 20) {
            c.chassis->setMotorSpeeds(0, 0);
            enteringState = 1;
            state = WAIT;
            nextState = DUMPPOISON;
            waitTime = 3000;
        }
            
    break;

    case DUMPPOISON:

        if (enteringState) {
            enteringState = 0;
            c.poison.down();
        }

        if (c.poison.done()) {
            enteringState = 1;
                state = WAIT;
                nextState = DRIVETOCOLLISION;
                waitTime = 2000;
        }
            
    break;

    case DRIVETOCOLLISION:
         
        c.chassis->FollowAprilTag(0, 10);
        
        if (c.tapper.CheckTap()) {
            enteringState = 1;
            state = DIE;
        }
            
    break;

    case DIE:

        c.simpleLED.off();
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