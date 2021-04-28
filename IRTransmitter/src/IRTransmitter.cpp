#pragma once
#include <Arduino.h>
#include <Romi32U4.h>

Romi32U4Motors motors;  
const int transmitter= 4;

void setup(){
pinMode(transmitter, INPUT);    
OCR1C = 0;


}
unsigned long start = millis();
bool status = 1;

void loop(){


 
 if(millis()-start>250){

    status = !status;
    if(status){
        OCR1C = 0;
    }
    else{
         OCR1C = 210;
    }
    start = millis();
 } 


}