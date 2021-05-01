#include "commonCode.h"

IRDecoder decoder(14);

void commonCode::setup(){
    Serial.begin(115200);
    chassis.setup();
    decoder.init();
}

void commonCode::loop(){
    chassis.loop();
    remoteCode = decoder.getKeyCode();
}