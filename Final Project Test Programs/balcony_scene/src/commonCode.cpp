#include "commonCode.h"

IRDecoder decoder(14);

void commonCode::setup(){
    // Serial.begin(115200); runs in main
    chassis.setup();
    decoder.init();
    // ledmanager.setup();
}

void commonCode::loop(){
    chassis.loop();
    remoteCode = decoder.getKeyCode();
}