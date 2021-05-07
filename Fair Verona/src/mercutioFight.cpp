#include "mercutioFight.h"

void mercutioFight::setup(){
    c.setup();

    state = WAIT;
    nextState = CIRCLE;
    waitTime = 3000;
}

void mercutioFight::loop(){
    if(millis() - printTime > 500){
        // Serial.print(c.chassis->theta);
        // Serial.print("\n");
    }

    c.loop();

    switch(state) {

        case IDLE:

        break;

        case CIRCLE:
            if(enteringState) {
                enteringState = 0;
                c.chassis->driveCircle(18.0, 25.0);
            }

            if (c.chassis->theta > (6 * 3.14)) {
                enteringState = 1;
                c.chassis->setMotorSpeeds(0, 0);
                state = WAITTODIE;
            }
            
        break;

        case WAITTODIE:
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