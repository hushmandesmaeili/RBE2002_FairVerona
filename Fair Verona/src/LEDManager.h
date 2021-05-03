#pragma once

#include "Romi32U4.h"

class LEDManager{

    public:
    void fadeIn(void);
    void fadeOut(void);
    void setup();
    void loop();
    bool fadeInBool = false;
    bool fadeOutBool = false;
    
    
    private: 
   const int LEDPin = 7;
   unsigned long targetInterval = 1000;
   unsigned long previousTime;
   const int changeInSupply = 5;
   int currentVal;
    };

   


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