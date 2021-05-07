#include "commonCode.h"

IRDecoder decoder(14);

void commonCode::setup(){
    chassis = &chassis->Instance();
    // Serial.begin(115200); runs in main
    chassis->setup();
    if (tapDetectorOn)
        tapper.Init();
    decoder.init();
    poison.setup();
    simpleLED.setup();
    // ledmanager.setup();
}

void commonCode::loop(){
    chassis->loop();
    remoteCode = decoder.getKeyCode();
    // ledmanager.loop();
}