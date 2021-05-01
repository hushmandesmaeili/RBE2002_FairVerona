#include "Chassis.h"

PIDController leftMotorController(12, 1, 0, 300); //start with  Kp = 1
PIDController rightMotorController(12, 1, 0, 300); //start with  Kp = 1
volatile uint8_t PIDController::readyToPID = 0; //a flag that is set when the PID timer overflows

PIDController wallFollow(1, 0.1, 1, 75); //wall follow controller
ourTimer wallFollowTimer(50); //every 50ms

volatile int16_t countsLeft = 0;
volatile int16_t countsRight = 0;
Romi32U4Encoders encoders;

void Chassis::setup(){
    noInterrupts(); //disable interupts while we mess with the Timer4 registers
  
    //sets up timer 4
    TCCR4A = 0x00; //disable some functionality -- no need to worry about this
    TCCR4B = 0x0B; //sets the prescaler -- look in the handout for values
    TCCR4C = 0x04; //toggles pin 6 at the timer frequency
    TCCR4D = 0x00; //normal mode

    OCR4C = 249;   //TOP goes in OCR4C 
    timestepMS = 16; //should correspond to your choice for TOP

    TIMSK4 = 0x04; //enable overflow interrupt

    interrupts(); //re-enable interrupts

    pinMode(sharpRead, INPUT); //read sharp IR

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

void Chassis::loop(){
    wallFollower();
    updateSpeeds();
    UpdatePitch();
}

void Chassis::wallFollower(void){
    //when wall follow timer triggers, set target speeds to feed to velocity controller
    if(wallFollowTimer.isExpired() && wallFollowEnable){ //for using sharp IR
        sampleCount++;
        lastSharpSamples[sampleCount % 5] = getDistance();
        float avg_distance = rollingAverage(lastSharpSamples);
        float error = avg_distance - targetDistance;

        turnEffort = wallFollow.ComputeEffort(error);
        targetSpeedLeft =  targetSpeed - turnEffort;
        targetSpeedRight = targetSpeed + turnEffort;
    }
}

void Chassis::updateSpeeds(void){
    //uses target speed values to make robot go at speed
    //currently always using velocity control
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

        int16_t errorLeft = targetSpeedLeft - speedLeft; //calculate the error
        float effortLeft = leftMotorController.ComputeEffort(errorLeft); //calculate effort from error

        int16_t errorRight = targetSpeedRight - speedRight; //calculate the error
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

        // Serial.print('\n');
      }
}


bool Chassis::UpdatePitch(void)
{
    if(onRamp){ //update onRamp flag
        if(getPitchAng() < lowerLimit) onRamp = 0;
    } else {
        if(getPitchAng() > upperLimit) onRamp = 1;
    }

 if(imu.getStatus() & 0x01)
  {
    imu.read();
    float predictGyro = estimatedPitchAng + (dataRateSec * (imu.g.y - Bias) * senseRad);
    float obsPitch = atan2((double)(imu.a.x - accXoffset),(double)(imu.a.z));
    estimatedPitchAng = kappa * predictGyro + (1 - kappa) * obsPitch;
    Bias = Bias + E*(predictGyro - obsPitch);  

    // Serial.print(estimatedPitchAng);
    // Serial.print('\t');
    // Serial.print(Bias);
    // Serial.print('\t');
    //  Serial.print(imu.a.x);
    // Serial.print('\t');
    // Serial.print(accXoffset);
    // Serial.print('\t');
    return true;    
  }
    else
        return false;

}

bool Chassis::checkIfOnRamp(){
    return(onRamp);
}

void Chassis::GetXAverage(void) {
    if(sampleSize < 200)
    {
        isCalibrating = true;
        if(imu.getStatus() & 0x01)
        {
            imu.read();
            accXoffset += imu.a.x;
            sampleSize++;
        }
    }
    else if(sampleSize == 200) {
        accXoffset /= 200.0;
        isCalibrating = false;
    }
}

bool Chassis::IsCalibrating(void) {
    return isCalibrating;
}

float Chassis::getPitchAng() { 
    return(estimatedPitchAng);
}

void Chassis::setMotorSpeeds(int left, int right){ //speeds -75 to 75
    targetSpeedLeft = left;
    targetSpeedRight = right;
}

void Chassis::startWallFollow(){
    wallFollowEnable = 1;
}

void Chassis::stopWallFollow(){
    wallFollowEnable = 0;
}

float Chassis::getDistance(){
  uint16_t adc_out = analogRead(sharpRead);
  float voltage_out = ((float) adc_out * VREF) / 1023;

  float distance = 15.1 / (voltage_out - 0.333);

  return distance;
}

float Chassis::rollingAverage(float arr[5]){
  float avg = 0;
  float sum = 0;

  int i;
  for (i = 0; i < 5; i++) sum += arr[i];

  if (sampleCount > 5) avg = sum / 5;
  else avg = sum / sampleCount;

  return avg;
}

// int16_t Chassis::getCountsLeft(void){
//     return(countsLeft);
// }

// int16_t Chassis::getCountsRight(void){
//     return(countsRight);
}

ISR(TIMER4_OVF_vect)
{
  //Capture a "snapshot" of the encoder counts for later processing
  countsLeft = encoders.getCountsLeft();
  countsRight = encoders.getCountsRight();

  PIDController::readyToPID = 1;
}