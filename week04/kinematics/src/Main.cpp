#include <Romi32U4.h>
#include "chassis.h"

Chassis chassis;


void setup() 
{
   Serial.begin(115200);
   chassis.Init();
   
}

void loop()
{
    chassis.UpdatePitch();
}