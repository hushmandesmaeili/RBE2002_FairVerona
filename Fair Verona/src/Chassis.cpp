#include "Chassis.h"

void Chassis::setup(){
    setMotorEfforts(0, 0);
}

void Chassis::loop(){
    setMotorEfforts();
}

int Chassis::getLeftEffort(){
    return(leftEffort);
}

int Chassis::getRightEffort(){
    return(rightEffort);
}

void Chassis::setMotorEfforts(int lEffort, int rEffort){
    leftEffort = lEffort; 
    rightEffort = rEffort;
    setMotorEfforts();
}

void Chassis::setMotorEfforts(){ //runs every loop
    motors.setEfforts(leftEffort, rightEffort);
}