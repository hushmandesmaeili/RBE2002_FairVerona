// #pragma once

// #include "Romi32U4.h"
// #include <Arduino.h>
// #include "ourTimer.h"

// class LEDManager{

//     public:
//         void alive(void);
//         void dead(void);
//         void fadeIn(void);
//         void fadeOut(void);
//         void setup(void);
//         void loop(void);
//         bool fadeInBool = false;
//         bool fadeOutBool = false;
//     private: 
//         const int LEDPin = 5;
//         unsigned long previousTime;
//         const int changeInBrightness = 5;
//      //   int brightness = 0;
//         const int ALIVE = 249;
//         const int DEAD = 0;
// };

   


// unsigned long start = millis();
// bool status = 1;

// void loop(){


 
//  if(millis()-start>250){

//     status = !status;
//     if(status){
//         OCR1C = 0;
//     }
//     else{
//          OCR1C = 20;
//     }
//     start = millis();
//  } 