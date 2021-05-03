#include "LEDManager.h"

void LEDManager::setup(){
    pinMode(LEDPin, OUTPUT);    
}

void LEDManager::fadeIn(void){

        if(millis() - previousTime > targetInterval){
         analogWrite(LEDPin, currentVal);
         currentVal += changeInSupply;
         previousTime = millis();
        }       
    
       
}
void LEDManager::fadeOut(void){

 if(millis() - previousTime > targetInterval){
         analogWrite(LEDPin, currentVal);
         currentVal-=changeInSupply;
          previousTime = millis();

    }
}

void LEDManager::loop(){
    if(fadeInBool){
        fadeIn();
        if( currentVal == 0) fadeInBool = false;
    }
    if(fadeOutBool){
        fadeOut();
        if( currentVal == 255) {
            fadeInBool = true;
            currentVal = 0;
        }
    }
}