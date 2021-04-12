/*
 * Code for using TCC4 for precision PID timing.
 * You'll need to set TOP to set the interval
 * 
 * This code adds the ability to tune the gains and change the targets
 */

#include <Romi32U4.h>
#include <stdio.h>
#include <Arduino.h>

#include "serial_comm.h"
#include "PIDcontroller.h"
#include "ourTimer.h"
#include "reflectance.h"

Romi32U4ButtonA buttonA;
reflectance lineSensor;

PIDController leftMotorController(12, 1, 0, 300); //start with  Kp = 1
PIDController rightMotorController(12, 1, 0, 300); //start with  Kp = 1
volatile uint8_t PIDController::readyToPID = 0; //a flag that is set when the PID timer overflows

PIDController wallFollow(1, 0.1, 1, 75); //wall follow controller
ourTimer wallFollowTimer(50); //every 50ms

const uint8_t sharpRead = 18;
const float VREF = 5.0;
float lastSharpSamples[5];
uint16_t sampleCount = 0;

uint16_t targetDistance = 25;
int16_t turnEffort = 0;
int16_t targetSpeed = 15;

Romi32U4Motors motors;
Romi32U4Encoders encoders;

volatile int16_t countsLeft = 0;
volatile int16_t countsRight = 0;

void setup()
{
  Serial.begin(115200);
  //while(!Serial) {}  //IF YOU DON'T COMMENT THIS OUT, YOU MUST OPEN THE SERIAL MONITOR TO START
  Serial.println("Hi");

  noInterrupts(); //disable interupts while we mess with the Timer4 registers
  
  //sets up timer 4
  TCCR4A = 0x00; //disable some functionality -- no need to worry about this
  TCCR4B = 0x0B; //sets the prescaler -- look in the handout for values
  TCCR4C = 0x04; //toggles pin 6 at the timer frequency
  TCCR4D = 0x00; //normal mode

  /*
   * EDIT THE LINE BELOW WITH YOUR VALUE FOR TOP
   */

  OCR4C = 221;   //TOP goes in OCR4C 

  TIMSK4 = 0x04; //enable overflow interrupt
  
  interrupts(); //re-enable interrupts

  // pinMode(6, OUTPUT); //COMMENT THIS OUT TO SHUT UP THE PIEZO!!!

  pinMode(sharpRead, INPUT); //read sharp IR
  lineSensor.setup();
}

float targetLeft = 15; //max speed is around 75
float targetRight = 15;

float getDistance(){
  uint16_t adc_out = analogRead(sharpRead);
  float voltage_out = ((float) adc_out * VREF) / 1023;

  float distance = 15.1 / (voltage_out - 0.333);

  return distance;
}

float rollingAverage(float arr[5]){
  float avg = 0;
  float sum = 0;

  int i;
  for (i = 0; i < 5; i++) sum += arr[i];

  if (sampleCount > 5) avg = sum / 5;
  else avg = sum / sampleCount;

  return avg;
}

typedef enum {
  IDLE,
  WAITING,
  WALLING
} State;
State state = IDLE;
State lastState = IDLE;

uint32_t waitStart = 0;

void loop() 
{ 
  switch(state){
    case IDLE:
    lineSensor.set();
      motors.setEfforts(0, 0);
      if(buttonA.getSingleDebouncedPress()){
        if(lastState == IDLE){
          state = WAITING;
          lastState = IDLE;
          waitStart = millis();
        } else {
          state = WALLING;
          lastState = IDLE;
        }
      }
    break;
    case WAITING:
      if(millis() - waitStart >= 1000){
        state = WALLING;
        lastState = IDLE;
      }
    break;
    case WALLING:
      if(lineSensor.both()){
        state = IDLE;
        lastState = WALLING;
        break;
      }
      if(wallFollowTimer.isExpired()){ //for using sharp IR
        sampleCount++;
        lastSharpSamples[sampleCount % 5] = getDistance();
        float avg_distance = rollingAverage(lastSharpSamples);
        float error = avg_distance - targetDistance;

        turnEffort = wallFollow.ComputeEffort(error);
        targetLeft =  targetSpeed - turnEffort;
        targetRight = targetSpeed + turnEffort;
      }

      if(PIDController::readyToPID) //timer flag set
      {
        // reset the flag
        PIDController::readyToPID = 0;
        
        // for tracking previous counts
        static int16_t prevLeft = 0;
        static int16_t prevRight = 0;

        /*
        * Do PID stuffs here. Note that we turn off interupts while we read countsLeft/Right
        * so that it won't get accidentally updated (in the ISR) while we're reading it.
        */
        noInterrupts();

        int16_t speedLeft = countsLeft - prevLeft;
        prevLeft = countsLeft;

        int16_t speedRight = countsRight - prevRight;
        prevRight = countsRight;

        interrupts();

        int16_t errorLeft = targetLeft - speedLeft; //calculate the error
        float effortLeft = leftMotorController.ComputeEffort(errorLeft); //calculate effort from error

        int16_t errorRight = targetRight - speedRight; //calculate the error
        float effortRight = rightMotorController.ComputeEffort(errorRight); //calculate effort from error
        
        // motors.setEfforts(effortLeft, 0); 
        motors.setEfforts(effortLeft, effortRight); 

        // Serial.print(millis());
        // Serial.print('\t');
        // Serial.print(targetLeft);
        // Serial.print('\t');
        // Serial.print(speedLeft);
        // Serial.print('\t');
        // Serial.print(effortLeft/10.0); //divide effort by 10 for better plotting

        // //you may want to add more serial printout here for testing

        // Serial.print('\n');
      }

      /* for reading in gain settings
      * CheckSerialInput() returns true when it gets a complete string, which is
      * denoted by a newline character ('\n'). Be sure to set your Serial Monitor to 
      * append a newline
      */
      if(CheckSerialInput()) {ParseSerialInput();}
    break;
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
