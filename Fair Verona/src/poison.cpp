#include "poison.h"

void Poison::setup(void){
    servo.Init();
    servo.Attach(); 
    servo.SetMinMaxUS(900, 2100); 
    pinMode(servoPotPin, INPUT); //servo input pin
}

void Poison::up(){
    servo.Write(1800);
    setPoint = UP;
}

void Poison::down(){
    servo.Write(900);
    setPoint = DOWN;
}

bool Poison::done(){
    int servoVal = analogRead(servoPotPin);
    return (((setPoint - tolerance) < servoVal) && (servoVal < (setPoint + tolerance)));    
}

int Poison::getposition(){
    return(analogRead(servoPotPin));
}




