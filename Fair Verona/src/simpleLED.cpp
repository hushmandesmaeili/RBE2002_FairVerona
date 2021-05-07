#include "simpleLED.h"

void SimpleLED::setup(){
    pinMode(LEDPin, OUTPUT);
}

void SimpleLED::on(){
    digitalWrite(LEDPin, HIGH);
}

void SimpleLED::off(){
    digitalWrite(LEDPin, LOW);
}