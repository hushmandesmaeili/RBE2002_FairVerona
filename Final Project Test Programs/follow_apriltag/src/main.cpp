#include <Arduino.h>
#include <Romi32U4.h>

#include <chassis.h>

Chassis chassis;

void setup() {
    
    Serial.begin(115200);
    delay(1000);
  
    chassis.Init();

}

void loop() 
{
    // chassis.FollowAprilTag(17);
    // chassis.DetectAprilTag();

    if (chassis.DetectAprilTag() == 1) {
        chassis.FollowAprilTag(10);
    } else if (chassis.DetectAprilTag() == 3) {
        chassis.FollowAprilTag(15);
    }

    if(PIDController::readyToPID) //timer flag set
    {
    // reset the flag
        PIDController::readyToPID = 0;
        chassis.UpdateSpeeds();
    }
}