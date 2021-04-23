#include "chassis.h"

#include <math.h>

volatile int16_t countsLeft = 0;
volatile int16_t countsRight = 0;

Romi32U4Encoders encoders;
Romi32U4Motors motors;
LSM6 imu; 

Chassis::Chassis(void) 
    //we'll use the member initializer capabilities of C++ to set up the controllers
    : leftMotorController(5, 1, 0, 500), rightMotorController(5, 1, 0, 500)  
    {
        SetTargetSpeeds(0, 0);
    }

void Chassis::Init(void)
{  
    noInterrupts(); //disable interupts while we mess with the Timer4 registers
  
    //sets up timer 4
    TCCR4A = 0x00; //disable some functionality -- no need to worry about this
    TCCR4B = 0x0B; //sets the prescaler -- look in the handout for values
    TCCR4C = 0x04; //toggles pin 6 at the timer frequency
    TCCR4D = 0x00; //normal mode

    /*
    * EDIT THE LINE BELOW WITH YOUR VALUE FOR TOP
    */

    OCR4C = 249;   //TOP goes in OCR4C 
    timestepMS = 16; //should correspond to your choice for TOP

    TIMSK4 = 0x04; //enable overflow interrupt

    interrupts(); //re-enable interrupts

    //pinMode(6, OUTPUT); //COMMENT THIS OUT TO SHUT UP THE PIEZO!!!

     Wire.begin();

  if (!imu.init())
  {
    // Failed to detect the LSM6.
    ledRed(1);
    while(1)
    {
      Serial.println(F("Failed to detect the LSM6."));
      delay(100);
    }
  }

  imu.enableDefault();

  // Set the gyro full scale and data rate
  imu.setGyroDataOutputRate(LSM6::ODR13);

  // Set the accelerometer full scale and data rate
  imu.setAccDataOutputRate(LSM6::ODR13);
}

// void Chassis::UpdatePose(void)
// {
//     //TO BE COMPLETED BY THE STUDENT

//     //conversion from ticks/interval to cm/sec
//     float conversionToCMPerSec = 1; //YOU'LL NEED TO CALCULATE THIS VALUE
    
//     float spLeft = speedLeft * conversionToCMPerSec;
//     float spRight = speedRight * conversionToCMPerSec;

//     //average speed
//     float u_0 = 0; //YOU'LL NEED TO ADD THIS EXPRESSION

//     //omega
//     float omega = 0; //YOU'LL NEED TO ADD THIS EXPRESSION

//     //simple first-order method -- not sufficient for class
//     float dt = timestepMS / 1000.; //SET timestepMS IN THE CONSTRUCTOR

//     //YOU'LL NEED TO CALCULATE THESE
//     x += 0;
//     y += 0;
//     theta += 0;

//     // Serial.print(millis());
//     // Serial.print('\t');
//     // Serial.print(x);
//     // Serial.print('\t');
//     // Serial.print(y);
//     // Serial.print('\t');
//     // Serial.print(theta); 
//     // Serial.print('\n');
// }

void Chassis::UpdateSpeeds(void)
{
    /*
     * Do PID stuffs here. Note that we turn off interrupts while we read countsLeft/Right
     * so that it won't get accidentally updated (in the ISR) while we're reading it.
     */
    noInterrupts();

    speedLeft = countsLeft - prevEncLeft;
    prevEncLeft = countsLeft;
    speedRight = countsRight - prevEncRight;
    prevEncRight = countsRight;

    interrupts();

    int16_t errorLeft = targetSpeedLeft - speedLeft; //calculate the error
    float effortLeft = leftMotorController.ComputeEffort(errorLeft); //calculate effort from error
    
    motors.setLeftEffort(effortLeft); 

    int16_t errorRight = targetSpeedRight - speedRight; //calculate the error
    float effortRight = rightMotorController.ComputeEffort(errorRight); //calculate effort from error
    
    motors.setRightEffort(effortRight);

//     Serial.print(millis());
//     Serial.print("\t");
//     Serial.print(speedLeft);
//     Serial.print("\t");
//     Serial.print(speedRight);
//     Serial.print("\n");
}

void Chassis::UpdatePose() {
    float ticks_per_cm = ticks_per_rotation / (PI * wheel_diam); 
    // float dt = timestepMS / 1000; 
    float angVel = (speedRight - speedLeft) / (wheel_track * ticks_per_cm); //in rads per interval
    float speedCenter = (speedRight + speedLeft) / (2.0 * ticks_per_cm); //in cm per interval
    float oldTheta = theta;
    theta = oldTheta + angVel;
    float thetaStar = (oldTheta + theta) / 2.0;
    x += speedCenter * cos(thetaStar); //in cm
    y += speedCenter * sin(thetaStar); //in cm

    Serial.print(x);
    Serial.print("\t");
    Serial.println(y);
}

void Chassis::MoveToPoint(void) {
    float errorDistance = sqrt(pow((x - x_target), 2) + pow((y - y_target), 2));
    float internalTargetTheta = atan2(y_target - y, x_target - x);
    float errorTheta =  internalTargetTheta - theta;

    targetSpeedLeft = kpD * errorDistance - kpTheta * errorTheta;
    targetSpeedRight = kpD * errorDistance + kpTheta * errorTheta;
}
bool Chassis::AreWeThere(void)
{
    return (abs(x - x_target) <= BUFFER && abs(y - y_target) <= BUFFER);
}

bool Chassis::UpdatePitch(void)
{


 if(imu.getStatus() & 0x01)
  {
    imu.read();
    float predictGyro = estimatedPitchAng + (dataRateSec * imu.g.y * senseRad);
    float obsPitch = atan2((double)(imu.a.x),(double)(imu.a.z));
    estimatedPitchAng = kappa * predictGyro + (1 - kappa) * obsPitch;

    Serial.print(millis());
    Serial.print('\t');
    Serial.print(obsPitch);
    Serial.print('\t');
    Serial.print(estimatedPitchAng);
    Serial.print('\n');
    return true;


    
    // // Accelerometer
    //   Serial.print(imu.a.x);
    //   Serial.print(' ');
    //   Serial.print(imu.a.y);
    //   Serial.print(' ');
    //   Serial.print(imu.a.z);
    //   Serial.print(' ');
    

    
    // // Gyros
    //   Serial.print(imu.g.x);
    //   Serial.print(' ');
    //   Serial.print(imu.g.y);
    //   Serial.print(' ');
    //   Serial.print(imu.g.z);
    //   Serial.print(' ');
    

    
  }
    else
{
    return false;
}

}




/*
 * ISR for timing. On overflow, it takes a 'snapshot' of the encoder counts and raises a flag to let
 * the main program it is time to execute the PID calculations.
 */
ISR(TIMER4_OVF_vect)
{
  //Capture a "snapshot" of the encoder counts for later processing
  countsLeft = encoders.getCountsLeft();
  countsRight = encoders.getCountsRight();

  PIDController::readyToPID = 1;
}


