#include "LEDManager.h"

ourTimer timerLED(30);

void LEDManager::setup() {
    pinMode(LEDPin, OUTPUT);
    timerLED.reset();
}

void LEDManager::fadeIn(void) {

        // if(millis() - previousTime > targetInterval){
        //  analogWrite(LEDPin, currentVal);
        //  currentVal += changeInSupply;
        //  previousTime = millis();
        // }   

         if(timerLED.isExpired()) {
            analogWrite(LEDPin, currentVal);
            currentVal = currentVal + changeInSupply;
        }     
        // delay(30);
    
       
}
void LEDManager::fadeOut(void) {

//  if(millis() - previousTime > targetInterval){
//          analogWrite(LEDPin, currentVal);
//          currentVal-=changeInSupply;
//           previousTime = millis();

//     }

    if(timerLED.isExpired()) {      
        analogWrite(LEDPin, currentVal);
        currentVal -= changeInSupply;
        // Serial.print(currentVal);
        // Serial.println();
    } 
}

void LEDManager::loop() {
    if(fadeInBool){
        fadeIn();
        if( currentVal >= 255) fadeInBool = false;
    }
    if(fadeOutBool){
        fadeOut();
        if( currentVal <= 0) {
            fadeOutBool = true;
            currentVal = 0;
        }
    }
    // fadeIn();
}