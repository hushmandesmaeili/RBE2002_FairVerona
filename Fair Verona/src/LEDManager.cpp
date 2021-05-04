#include "LEDManager.h"

const unsigned long targetInterval = 30;
ourTimer timerLED(targetInterval);

void LEDManager::setup() {
    pinMode(LEDPin, OUTPUT);
    analogWrite(LEDPin, 0);
    timerLED.reset();
}

void LEDManager::alive(void) {
    analogWrite(LEDPin, ALIVE);
}

void LEDManager::dead(void) {
    analogWrite(LEDPin, DEAD);
}

void LEDManager::fadeIn(void) {
    if(timerLED.isExpired()) {
        analogWrite(LEDPin, brightness);
        brightness = brightness + changeInBrightness;
    }        
}

void LEDManager::fadeOut(void) {
    if(timerLED.isExpired()) {      
        analogWrite(LEDPin, brightness);
        brightness = brightness - changeInBrightness;
    } 
}

void LEDManager::loop() {
    if(fadeInBool){
        fadeIn();
        if( brightness >= 255) fadeInBool = false;
    }
    if(fadeOutBool){
        fadeOut();
        if( brightness <= 0) {
            fadeOutBool = true;
            brightness = 0;
        }
    }
}