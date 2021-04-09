#include "reflectance.h"

void reflectance::setup(){
    pinMode(leftSensor, OUTPUT);
    pinMode(rightSensor, INPUT);
}

bool reflectance::both(){
    return(analogRead(leftSensor) >= blackDetect && analogRead(rightSensor) >= blackDetect);
}