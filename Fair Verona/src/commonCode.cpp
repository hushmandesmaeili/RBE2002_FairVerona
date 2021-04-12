#include "commonCode.h"

void commonCode::setup(){
    chassis.setup();
}

void commonCode::loop(){
    chassis.loop();
}

int commonCode::getRightEffort(){
    return(chassis.getRightEffort());
}

int commonCode::getLeftEffort(){
    return(chassis.getLeftEffort());
}