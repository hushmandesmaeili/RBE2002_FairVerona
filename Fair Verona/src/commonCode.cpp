#include "commonCode.h"

void commonCode::setup(){
    Serial.begin(115200);
    chassis.setup();
}

void commonCode::loop(){
    chassis.loop();
}