#include <Romi32U4.h>
#include "chassis.h"
Romi32U4ButtonA buttonA;
Romi32U4ButtonB buttonB;
Romi32U4ButtonC buttonC;
Chassis chassis;

const int LED_PIN = 17;

bool onRamp = 0; //true if ramp
float upperLimit = 0.1;
float lowerLimit = 0.05;

bool abovePitch(){
    if(onRamp){
        if(chassis.getPitchAng() < lowerLimit) onRamp = 0;
    } else {
        if(chassis.getPitchAng() > upperLimit) onRamp = 1;
    }
    return(onRamp);
}

typedef enum {
    WAITFORBUTTON,
    WAITFOR1S,
    DRIVETORAMP,
    CLIMBING,
    STOP
} State;
State state = WAITFORBUTTON;
unsigned long startTime = 0;

void stateMachine() {
    switch(state){
        case WAITFORBUTTON:
            if(buttonC.getSingleDebouncedPress()){
                state = WAITFOR1S;
                startTime = millis();
            }
        break;
        case WAITFOR1S:
            if(millis() - startTime > 1000) state = DRIVETORAMP;
        break;
        case DRIVETORAMP:
            // chassis.SetTargetSpeeds(50.0, 50.0);
            chassis.motors.setEfforts(50, 50);
            if(abovePitch()) state = CLIMBING;
        break;
        case CLIMBING:
            if(!abovePitch()) state = STOP;
            digitalWrite(LED_PIN, 1);
        break;
        case STOP:
            chassis.motors.setEfforts(0, 0);
            digitalWrite(LED_PIN, 0);
            // chassis.SetTargetSpeeds(0, 0);
        break;
    }
}

void setup() 
{
   Serial.begin(115200);
   chassis.Init();
   pinMode(LED_PIN, OUTPUT);
}

void loop()
{
    if(buttonA.getSingleDebouncedPress() || chassis.IsCalibrating()) {
        chassis.GetXAverage();
    } else {
        stateMachine();
        chassis.UpdatePitch();
    } 
}