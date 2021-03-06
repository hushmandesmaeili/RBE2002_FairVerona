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

const float radius = 50;  // in cm
const float angleInt = PI / 8;  // angle interval
int count = 0;

void setup()
{
  Serial.begin(115200);
  //while(!Serial) {}  //IF YOU DON'T COMMENT THIS OUT, YOU MUST OPEN THE SERIAL MONITOR TO START
  Serial.println("Hi");

  chassis.Init();
}

enum STATE {DR_IDLE, DR_WAITING, DR_TARGET, DR_DRIVING};
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
        if(waitTimer.isExpired()){
          // if(destination == DEST_A) //drive straight
          // {
            // chassis.SetTargetSpeeds(12.2, 12.2);  //YOU'LL WANT TO CHANGE THESE
            // waitTimer.reset(5000);          //YOU'LL WANT TO CHANGE THESE
            // chassis.SetTargetPosition(30.0, 30.0);

            state = DR_TARGET;
          // }
        //   else if(destination == DEST_B) //spin in place
        //   {
        //   //   chassis.SetTargetSpeeds(-12.2, 12.2);  //YOU'LL WANT TO CHANGE THESE
        //   //   waitTimer.reset(4000);          //YOU'LL WANT TO CHANGE THESE

        //     chassis.SetTargetPosition(60.0, 0.0);
        //     state = DR_DRIVING;
        //   }
        //   else if(destination == DEST_C) //curl
        //   {
        //   //   chassis.SetTargetSpeeds(8.8, 13.2);  //YOU'LL WANT TO CHANGE THESE
        //   //   waitTimer.reset(5000);          //YOU'LL WANT TO CHANGE THESE

        //     chassis.SetTargetPosition(30.0, -30.0);
        //     state = DR_DRIVING;
        //   }
        }
      break;
      case DR_TARGET:
        float xt = radius * cos(angleInt * count);
        float yt = radius * sin(angleInt * count);
        chassis.SetTargetPosition(xt, yt);
      case DR_DRIVING:
        chassis.MoveToPoint();
        if(chassis.AreWeThere()) {
          count++;
          state = DR_TARGET;
        }
      break;;                                                                                                                                                                                  
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

  HandleTimerExpired();

  if(PIDController::readyToPID) //timer flag set
  {
    // reset the flag
    PIDController::readyToPID = 0;
    chassis.UpdateSpeeds();  
    chassis.UpdatePose();
  }

  // if(waitTimer.isExpired()) HandleTimerExpired();
}
