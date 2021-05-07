// #include "LEDManager.h"

// const unsigned long targetInterval = 30;
// ourTimer timerLED(targetInterval);

// void LEDManager::setup() {
//     pinMode(LEDPin, OUTPUT);
//     alive();
//     timerLED.reset();
// }

// void LEDManager::alive(void) {
//    // analogWrite(LEDPin, ALIVE);
//     OCR4A = 249;
// }

// void LEDManager::dead(void) {
//    // analogWrite(LEDPin, DEAD);
//    OCR4A = 0;
// }

// void LEDManager::fadeIn(void) {
//     if(timerLED.isExpired()) {
//       //  analogWrite(LEDPin, brightness);
//          OCR4A += changeInBrightness;
//     }        
// }

// void LEDManager::fadeOut(void) {
//     if(timerLED.isExpired()) {      
//       //  analogWrite(LEDPin, brightness);
//         OCR4A -= changeInBrightness;
//     } 
// }

// void LEDManager::loop() {
//     if(fadeInBool){
//         fadeIn();
//         if( OCR4A>= 249) fadeInBool = false;
           
            
//     }
//     if(fadeOutBool){
//         fadeOut();
//         if( OCR4A <= 0) {
//             fadeOutBool = true;
           
//         }
//     }
// }