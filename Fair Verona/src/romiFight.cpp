#include "romiFight.h"

void romiFight::setup(){
    c.setup();
    c.chassis->detectCollisionEnable = 1;
}

void romiFight::loop(){
    c.loop();
    if(millis() - printTime > 500){
        Serial.print(c.chassis->getHasCollided());
        printTime = millis();
    }
}