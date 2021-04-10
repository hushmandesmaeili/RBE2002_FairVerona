#include "reflectance.h"

void reflectance::setup(){
    pinMode(leftSensor, INPUT);
    pinMode(rightSensor, INPUT);
}

bool reflectance::both(){
    return(analogRead(leftSensor) >= blackDetect && analogRead(rightSensor) >= blackDetect);
}

void reflectance::set() {
// Serial.begin(115200);
int sensorLeft(analogRead(A3));
int sensorRight(analogRead(A2));
// Serial.print(sensorLeft);
// Serial.print(",");
// Serial.println(sensorRight);
}


