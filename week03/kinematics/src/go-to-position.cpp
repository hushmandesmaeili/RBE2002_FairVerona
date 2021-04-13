/*
 * Code for using TCC4 for precision PID timing.
 * You'll need to set TOP to set the interval
 * 
 * This code adds the ability to tune the gains and change the targets
 */

#include <Romi32U4.h>
#include "chassis.h"
#include "Timer.h"

//const uint8_t IR_PIN = A6;

Romi32U4ButtonA buttonA;
Romi32U4ButtonB buttonB;
Romi32U4ButtonC buttonC;
Chassis chassis;

Timer waitTimer(1000);

void setup()
{
  Serial.begin(115200);
  //while(!Serial) {}  //IF YOU DON'T COMMENT THIS OUT, YOU MUST OPEN THE SERIAL MONITOR TO START
  Serial.println("Hi");

  chassis.Init();
}

enum STATE {DR_IDLE, DR_WAITING, DR_DRIVING};
STATE state = DR_IDLE;

enum DESTINATION {DEST_NONE, DEST_A, DEST_B, DEST_C};
DESTINATION destination = DEST_NONE;

void HandleButton(DESTINATION d)
{
  Serial.println(d);
  destination = d;
  waitTimer.reset(1000);

  state = DR_WAITING;
}

void HandleTimerExpired(void)
{
    switch(state)
    {
      case DR_WAITING:
        if(destination == DEST_A) //drive straight
        {
          chassis.SetTargetSpeeds(1, 1);  //YOU'LL WANT TO CHANGE THESE
          waitTimer.reset(1000);          //YOU'LL WANT TO CHANGE THESE

          state = DR_DRIVING;
        }
        else if(destination == DEST_B) //spin in place
        {
          chassis.SetTargetSpeeds(1, 1);  //YOU'LL WANT TO CHANGE THESE
          waitTimer.reset(1000);          //YOU'LL WANT TO CHANGE THESE

          state = DR_DRIVING;
        }
        else if(destination == DEST_C) //curl
        {
          chassis.SetTargetSpeeds(1, 1);  //YOU'LL WANT TO CHANGE THESE
          waitTimer.reset(1000);          //YOU'LL WANT TO CHANGE THESE

          state = DR_DRIVING;
        }
        break;
      default:
        chassis.SetTargetSpeeds(0, 0);
        state = DR_IDLE;
        destination = DEST_NONE;
    }
}

void loop() 
{
  if(buttonA.getSingleDebouncedPress()) HandleButton(DEST_A);
  if(buttonB.getSingleDebouncedPress()) HandleButton(DEST_B);
  if(buttonC.getSingleDebouncedPress()) HandleButton(DEST_C);

  if(PIDController::readyToPID) //timer flag set
  {
    // reset the flag
    PIDController::readyToPID = 0;
    chassis.UpdateSpeeds();  
    chassis.UpdatePose();
  }

  if(waitTimer.isExpired()) HandleTimerExpired();
}
