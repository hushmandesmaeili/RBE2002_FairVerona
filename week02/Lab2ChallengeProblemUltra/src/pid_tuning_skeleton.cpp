/*
 * Code for using TCC4 for precision PID timing.
 * You'll need to set TOP to set the interval
 * 
 * This code adds the ability to tune the gains and change the targets
 */

#include <Romi32U4.h>

#include "serial_comm.h"
#include "PIDcontroller.h"
#include "ourTimer.h"
#include <reflectance.h>

Romi32U4ButtonA buttonA;
reflectance lineSensor;

PIDController leftMotorController(12, 1, 0, 300); //start with  Kp = 1
PIDController rightMotorController(12, 1, 0, 300); //start with  Kp = 1
volatile uint8_t PIDController::readyToPID = 0; //a flag that is set when the PID timer overflows

PIDController wallFollow(1, 0.1, 1, 75); //wall follow controller
// PIDController wallFollow(0.9, 0, 0, 0); //wall follow controller
ourTimer wallFollowTimer(100); //every 100ms

volatile uint16_t pulseStart = 0;
volatile uint16_t pulseEnd = 0;

//define the states for the echo capture
enum PULSE_STATE {PLS_IDLE, PLS_WAITING_LOW, PLS_WAITING_HIGH, PLS_CAPTURED};

//and initialize to IDLE
volatile PULSE_STATE pulseState = PLS_IDLE;

//this may be most any pin, connect the pin to Trig on the sensor
const uint8_t trigPin = 18;

// const float VREF = 5.0;
float lastSamples[5];
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
  // Serial.println("Hi");

  noInterrupts(); //disable interupts while we mess with the control registers

  sampleCount = 0;
  
  //sets timer 3 to normal mode (16-bit, fast counter)
  TCCR3A = 0; 
  
  interrupts(); //re-enable interrupts

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

  pinMode(trigPin, OUTPUT);
  pinMode(13, INPUT); //explicitly make 13 an input

  wallFollowTimer.reset();
  lineSensor.setup();

}

float targetLeft = 15; //max speed is around 75
float targetRight = 15;

/*
 * Commands the ultrasonic to take a reading
 */
void CommandPing(int trigPin)
{
  cli(); //disable interrupts

  TIFR3 = 0x20; //clear any interrupt flag that might be there

  TIMSK3 |= 0x20; //enable the input capture interrupt
  TCCR3B |= 0xC0; //set to capture the rising edge on pin 13; enable noise cancel

  sei(); //re-enable interrupts

  //update the state and command a ping
  pulseState = PLS_WAITING_LOW;
  
  digitalWrite(trigPin, HIGH); //command a ping by bringing TRIG HIGH
  delayMicroseconds(10);      //we'll allow a delay here for convenience; it's only 10 us
  digitalWrite(trigPin, LOW);  //must bring the TRIG pin back LOW to get it to send a ping
}

/*
 * Returns the rolling average of array passed as argument
 */
float rollingAverage(float arr[5]) 
{
  float avg = 0;
  float sum = 0;
  
  int i;
  for (i = 0; i < 5; i++)
    sum += arr[i];

  //if condition to check if first 5 samples to adjust denominator in average calculation
  if (sampleCount > 5)
    avg = sum / 5;
  else
    avg = sum / sampleCount;

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
  
      if (wallFollowTimer.isExpired() && pulseState == PLS_IDLE) { //for using Ultrasonic
        CommandPing(trigPin); //command a ping
      }

      if(pulseState == PLS_CAPTURED) //we got an echo
      {

        //increment sample count
        sampleCount++;

        //update the state to IDLE
        pulseState = PLS_IDLE;

        /*
        * Calculate the length of the pulse (in timer counts!). Note that we turn off
        * interrupts for a VERY short period so that there is no risk of the ISR changing
        * pulseEnd or pulseStart. The way the state machine works, this wouldn't 
        * really be a problem, but best practice is to ensure that no side effects can occur.
        */
        noInterrupts();
        uint16_t pulseLengthTimerCounts = pulseEnd - pulseStart;
        interrupts();
        
        //EDIT THIS LINE: convert pulseLengthTimerCounts, which is in timer counts, to time, in us
        //You'll need the clock frequency and the pre-scaler to convert timer counts to time
        
        uint32_t pulseLengthUS = (pulseLengthTimerCounts * 4); //pulse length in us


        //EDIT THIS LINE AFTER YOU CALIBRATE THE SENSOR: put your formula in for converting us -> cm
        float distancePulse = pulseLengthUS / 58.0;    //distance in cm
        
        //store distancePulse to lastSamples array, using circular indexing for last five samples
        lastSamples[sampleCount % 5] = distancePulse;

        // float avg_distance = rollingAverage(lastSamples);
        
        // float error = avg_distance - targetDistance;
        float error = distancePulse - targetDistance;

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
        
        motors.setEfforts(effortLeft, effortRight); 
      }

      /* for reading in gain settings
      * CheckSerialInput() returns true when it gets a complete string, which is
      * denoted by a newline character ('\n'). Be sure to set your Serial Monitor to 
      * append a newline
      */
      // if(CheckSerialInput()) {ParseSerialInput();}
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

/*
 * ISR for input capture on pin 13. We can precisely capture the value of TIMER3
 * by setting TCCR3B to capture either a rising or falling edge. This ISR
 * then reads the captured value (stored in ICR3) and copies it to the appropriate
 * variable.
 */
ISR(TIMER3_CAPT_vect)
{
  if(pulseState == PLS_WAITING_LOW) //we're waiting for a rising edge
  {
    pulseStart = ICR3; //copy the input capture register (timer count)
    TCCR3B &= 0xBF;    //now set to capture falling edge on pin 13
    pulseState = PLS_WAITING_HIGH;
  }

  else if(pulseState == PLS_WAITING_HIGH) //waiting for the falling edge
  {
    pulseEnd = ICR3;
    pulseState = PLS_CAPTURED; //raise a flag to indicate that we have data
  }
}
