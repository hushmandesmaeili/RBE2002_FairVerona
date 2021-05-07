#include "simpleLED.h"

void SimpleLED::setup(){
    pinMode(LEDPin, OUTPUT);
    on();
}

void SimpleLED::on(){
    digitalWrite(LEDPin, HIGH);
}

void SimpleLED::off(){
    digitalWrite(LEDPin, LOW);
}