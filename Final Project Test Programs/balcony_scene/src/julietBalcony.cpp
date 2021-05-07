// #include "julietBalcony.h"
#include "commonCode.h"
#include "Chassis.h"

commonCode c;
Chassis chassis;
    typedef enum{
    IDLE,
    APPROACH,
    ONRAMP,
    TOPOFRAMP,
    SPINBACK,
    DOWNRAMP,
    ONFLOOR,
    TURNAWAY,
    DRIVEOFF,
    WAIT,
    STOP,
    TEST
    } State;
    State state;
    State nextState;

    bool enteringState = 1;

    bool goingDown;
    unsigned long timeLast, waitTime, printTime;
    float thetaLast = 0;
    int speed;

void setup(){
    // c.chassis.setup();
    c.setup();
    // c.chassis.checkRampEnable = 1;

    // // c.chassis.SetTargetPosition(60.0, 0.0);
    state = TEST;
    // // pinMode(13, OUTPUT);
}


void loop() {
    // if(millis() - printTime > 500){
    //     Serial.print(thetaLast);
    //     Serial.print('\t');
    //     Serial.print(chassis.theta);
    //     Serial.print('\n');
    //     // Serial.println(state);
    //     printTime = millis();
    //     // c.chassis.wallFollowDirection = !c.chassis.wallFollowDirection;
    // }

    // // c.loop();
    c.loop();

    switch(state){
        case TEST:
            if(enteringState){
                thetaLast = chassis.theta;
                c.chassis.setMotorSpeeds(-10, 10);
                enteringState = 0;
            }

            
        break;
    //     case IDLE:
    //         // while idling will calibrate when button a is pressed (blocking)
    //         if(c.buttonC.getSingleDebouncedPress() || c.chassis.IsCalibrating()) {
    //             c.chassis.GetXAverage();
    //         }

    //         // if(c.remoteCode == remotePlayPause) state = APPROACH;
    //         if(c.buttonB.getSingleDebouncedPress()) state = APPROACH;
    //     break;

    //     case APPROACH:
    //         if(enteringState) {
    //             c.chassis.wallFollowEnable = 1;
    //             c.chassis.wallFollowDirection = 1;
    //             enteringState = 0;
    //         }

    //         if(c.chassis.checkIfOnRamp()) {
    //             state = ONRAMP;
    //             enteringState = 1;
    //         }
    //     break;

    //     case ONRAMP:
    //         if(enteringState){
    //             enteringState = 0;
    //         }

    //         if(!c.chassis.checkIfOnRamp()){
    //             c.chassis.wallFollowEnable = 0;
    //             c.chassis.setMotorSpeeds(0, 0);
    //             state = WAIT;
    //             nextState = TOPOFRAMP;
    //             waitTime = 1000;
    //             enteringState = 1;
    //         }
    //     break;

    //     case TOPOFRAMP:
    //         if(enteringState){
    //             enteringState = 0;
    //             thetaLast = c.chassis.theta;
    //             c.chassis.setMotorSpeeds(-3, 3);
    //         }

    //         if(c.chassis.theta - thetaLast > 25){
    //             c.chassis.setMotorSpeeds(0, 0);
    //             state = WAIT;
    //             nextState = SPINBACK;
    //             waitTime = 2000;
    //             enteringState = 1;
    //         }
    //     break;

    //     case SPINBACK:
    //         if(enteringState){
    //             enteringState = 0;
    //             thetaLast = c.chassis.theta;
    //             c.chassis.setMotorSpeeds(3, -3);
    //         }

    //         if(c.chassis.theta - thetaLast < -25){
    //             c.chassis.setMotorSpeeds(0, 0);
    //             state = WAIT;
    //             nextState = DOWNRAMP;
    //             waitTime = 1000;
    //             enteringState = 1;
    //         }
    //     break;

    //     case DOWNRAMP:
    //         if(enteringState){
    //             enteringState = 0;
    //             c.chassis.wallFollowEnable = 1;
    //             c.chassis.wallFollowDirection = 0;
    //             // c.chassis.setMotorSpeeds(-10, -10);s
    //             goingDown = 0;
    //         }

    //         if(c.chassis.checkIfOnRamp()) goingDown = 1;
    //         else if (goingDown == 1) { //if we just came off the ramp
    //             state = ONFLOOR;
    //             enteringState = 1;
    //         }
    //     break;

    //     case ONFLOOR:
    //         if(enteringState){
    //             c.chassis.wallFollowEnable = 0;
    //             enteringState = 0;
    //             timeLast = millis();
    //             c.chassis.setMotorSpeeds(-10, -10);
    //         }

    //         if(millis() - timeLast > 3000){
    //             enteringState = 1;
    //             state = TURNAWAY;
    //         }
    //     break;

        // case TURNAWAY:
        //     if(enteringState){
        //         enteringState = 0;
        //         c.chassis.resetPose();
        //         c.chassis.SetTargetPosition(0.0, 0.0, 1.57);
        //     }

        //     c.chassis.MoveToPoint();

        //     if(c.chassis.AtTargetPosition()){
        //         c.chassis.setMotorSpeeds(0, 0);
        //         state = WAIT;
        //         nextState = DRIVEOFF;
        //         waitTime = 1000;
        //         enteringState = 1;
        //     }

        // break;

        // case DRIVEOFF:

        //     // c.loop();
        //     // c.chassis.setMotorSpeeds(10, 10);

        //     if (enteringState) {
        //         enteringState = 0;
        //         c.chassis.resetPose();
        //         c.chassis.SetTargetPosition(100.0, 0.0, 0.0);
                
        //     }

        //     c.chassis.MoveToPoint();

        //     if (c.chassis.AtTargetPosition()) {
        //         state = WAIT;
        //         nextState = STOP;
        //         waitTime = 3000;
        //     }
        //     // else
            
        // break;

        // case STOP:
        //     if(enteringState){
        //         enteringState = 0;
        //         c.chassis.setMotorSpeeds(0, 0);
        //     }
        // break;

        // case WAIT:
        //     if(enteringState){
        //         enteringState = 0;
        //         timeLast = millis();
        //     }

        //     if(millis() - timeLast > waitTime){
        //         state = nextState;
        //         enteringState = 1;
        //         waitTime = 0;
        //     }
        // break;
    }
}
