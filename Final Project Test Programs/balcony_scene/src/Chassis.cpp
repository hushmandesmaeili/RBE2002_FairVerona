#include "Chassis.h"

PIDController leftMotorController(12, 1, 0, 300); //start with  Kp = 1
PIDController rightMotorController(12, 1, 0, 300); //start with  Kp = 1
volatile uint8_t PIDController::readyToPID = 0; //a flag that is set when the PID timer overflows

PIDController wallFollow(3, 0.1, 0.1, 15); //wall follow controller
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
    // pinMode(sharpRead2, INPUT);

    Wire.begin();

    if (!imu.init())
    {
        // imu.init();
        // Failed to detect the LSM6.
        ledRed(1);
        // while(1)
        // {
        Serial.println(F("Failed to detect the LSM6."));
        delay(100);
        // }
    }

    imu.enableDefault();

    // Set the gyro full scale and data rate
    imu.setGyroDataOutputRate(LSM6::ODR13);

    // Set the accelerometer full scale and data rate
    imu.setAccDataOutputRate(LSM6::ODR13);
}

void Chassis::loop() {
    // if(wallFollowEnable) wallFollower();
    // // updateSpeeds();
    // UpdatePitch();
    // // updatePose();
    // if(checkRampEnable) checkRamp();
    // if(detectCollisionEnable) collisionDetect();



    if(PIDController::readyToPID) {
        PIDController::readyToPID = 0;
        updateSpeeds();
        updatePose(); 
    }

    // if (updatePoseEnable) 

    // if(wallFollowEnable) {
    //     manualSpeedsEnable = 0;
    // } else {
    //     manualSpeedsEnable = 1;
    // }
}

void Chassis::updateSpeeds(void){
    //uses target speed values to make robot go at speed
    //currently always using velocity control
    // if(PIDController::readyToPID) {
    //     PIDController::readyToPID = 0;    

        // for tracking previous counts
        static int16_t prevLeft = 0;
        static int16_t prevRight = 0;

        /*
        * Do PID stuffs here. Note that we turn off interupts while we read countsLeft/Right
        * so that it won't get accidentally updated (in the ISR) while we're reading it.
        */
        noInterrupts();

        speedLeft = countsLeft - prevLeft;
        prevLeft = countsLeft;

        speedRight = countsRight - prevRight;
        prevRight = countsRight;

        interrupts();

        int16_t errorLeft = targetSpeedLeft - speedLeft; //calculate the error
        float effortLeft = leftMotorController.ComputeEffort(errorLeft); //calculate effort from error

        int16_t errorRight = targetSpeedRight - speedRight; //calculate the error
        float effortRight = rightMotorController.ComputeEffort(errorRight); //calculate effort from error
        
        // motors.setEfforts(effortLeft, 0); 
        motors.setEfforts(effortLeft, effortRight); 
    // }
        // Serial.print(speedLeft);
        // Serial.print("\t");
        // Serial.print(speedRight); 
        // Serial.print("\n");
}

void Chassis::updatePose(void) {

    // if(PIDController::readyToPID) {
    //     PIDController::readyToPID = 0;

        float ticks_per_cm = ticks_per_rotation / (PI * wheel_diam); 
        float dt = timestepMS / 1000; 
        float angVel = (speedRight - speedLeft) / (wheel_track * ticks_per_cm); //in rads per interval
        float speedCenter = (speedRight + speedLeft) / (2.0 * ticks_per_cm); //in cm per interval
        float oldTheta = theta;
        theta = oldTheta + angVel;
        float thetaStar = (oldTheta + theta) / 2.0;
        x += speedCenter * cos(thetaStar); //in cm
        y += speedCenter * sin(thetaStar); //in cm
    // }
    // Serial.print(x);
    // Serial.print("\t");
    // Serial.println(y);
}

void Chassis::resetPose(void) {
    x = 0;
    y = 0;
    theta = 0;
}

void Chassis::MoveToPoint(void) {
    float errorDistance = sqrt(pow((x - x_target), 2) + pow((y - y_target), 2));
    float internalTargetTheta = atan2(y_target - y, x_target - x);
    float errorTheta =  internalTargetTheta - theta;
    float errorHeading = th_target - theta;

    // Serial.print(errorDistance);
    // Serial.print("\t");
    // Serial.print(errorTheta);
    // Serial.print("\n");

    targetSpeedLeft = kpD * errorDistance - kpTheta * errorTheta - kpFinalHeading * errorHeading;
    targetSpeedRight = kpD * errorDistance + kpTheta * errorTheta + kpFinalHeading * errorHeading;

    // targetSpeedLeft = kpD * errorDistance - kpTheta * errorTheta - kpFinalHeading * errorHeading;
    // targetSpeedRight = kpD * errorDistance + kpTheta * errorTheta + kpFinalHeading * errorHeading;

    Serial.print(targetSpeedLeft);
    Serial.print("\t");
    Serial.print(targetSpeedRight); 
    Serial.print("\n");

    // if (abs(targetSpeedLeft) > MAX_SPEED_LIMIT)
    //     targetSpeedLeft = MAX_SPEED_LIMIT;

    // if (abs(targetSpeedRight) > MAX_SPEED_LIMIT)
    //     targetSpeedRight = MAX_SPEED_LIMIT;
}

bool Chassis::AtTargetPosition(void) {
    bool retValue = (abs(x - x_target) <= BUFFER_TARGET_POSE 
        && abs(y - y_target) <= BUFFER_TARGET_POSE) 
        && abs(theta - th_target) <= BUFFER_FINAL_HEADING ;

    return retValue;
}

void Chassis::wallFollower(void){
    //when wall follow timer triggers, set target speeds to feed to velocity controller
    if(wallFollowTimer.isExpired()){ //for using sharp IR
        sampleCount++;
        lastSharpSamples[sampleCount % 5] = getDistance();
        float avg_distance = rollingAverage(lastSharpSamples);
        float error = avg_distance - targetDistance;
       
        turnEffort = wallFollow.ComputeEffort(error);
        if(wallFollowDirection){
            targetSpeedLeft =  targetSpeed + turnEffort;
            targetSpeedRight = targetSpeed - turnEffort;
        }
        else {
            targetSpeedLeft =  - (targetSpeed + turnEffort);
            targetSpeedRight = - (targetSpeed - turnEffort);
        }

       
    }
}

void Chassis::checkRamp(){
    if(onRamp) { //update onRamp flag
        if(getPitchAng() < lowerLimit) onRamp = 0;
    } else {
        if(getPitchAng() > upperLimit) onRamp = 1;
    }
}

bool Chassis::UpdatePitch(void) {
 if (imu.getStatus() & 0x01) {
    imu.read();
    float predictGyro = estimatedPitchAng + (dataRateSec * (imu.g.y - Bias) * senseRad);
    float obsPitch = atan2((double)(imu.a.x - accXoffset),(double)(imu.a.z));
    estimatedPitchAng = kappa * predictGyro + (1 - kappa) * obsPitch;
    Bias = Bias + E*(predictGyro - obsPitch);  
    
    return true;    
  }
    else
        return false;

}

bool Chassis::checkIfOnRamp() {
    return(onRamp);
}

bool Chassis::getHasCollided(){
    return(hasCollided);
}

void Chassis::collisionDetect(){
    if(imu.getStatus() & 0x01) {
        if(pitchLast - estimatedPitchAng > 10) hasCollided = 1;
        pitchLast = estimatedPitchAng;
    }
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
    return (estimatedPitchAng);
}

void Chassis::setMotorSpeeds(int left, int right) { //speeds -75 to 75
    // if (manualSpeedsEnable) {
        targetSpeedLeft = left;
        targetSpeedRight = right;
    // }
}

// void Chassis::startWallFollow(){
//     wallFollowEnable = 1;
// }

// void Chassis::stopWallFollow(){
//     wallFollowEnable = 0;
// }

float Chassis::getDistance(){
  uint16_t adc_out = analogRead(sharpRead);
  if(wallFollowDirection) adc_out = analogRead(sharpRead);
  else adc_out = analogRead(sharpRead2);
  float voltage_out = ((float) adc_out * VREF) / 1023;

  float distance = 15.1 / (voltage_out - 0.333); //TODO use other equation for other sensor

  return distance;
}

void Chassis::FollowAprilTag(float targetDistance) {

    float tDistance = targetDistance + CAMERA_OFFSET; //target distance plus camera offset from front
    
    uint8_t tagCount = getTagCount();
    if(tagCount) 
    {
        if(readTag(&tag)) {
            float errorDistance =  getDistanceCam(tag.w) - tDistance;
            float errorXTranslation = getDeltaCXCam(tag.cx);

            targetSpeedLeft = errorDistance * kp_distance - errorXTranslation * kp_alignment;
            targetSpeedRight = errorDistance * kp_distance + errorXTranslation * kp_alignment;

            // if (abs(targetSpeedLeft) > MAX_SPEED_LIMIT)
            //     targetSpeedLeft = MAX_SPEED_LIMIT;

            // if (abs(targetSpeedRight) > MAX_SPEED_LIMIT)
            //     targetSpeedRight = MAX_SPEED_LIMIT;

        }
    }
}

int Chassis::DetectAprilTag() {

    int retID;

    uint8_t tagCount = getTagCount();
    if(tagCount) 
    {
        if(readTag(&tag)) {
            retID = tag.id;
        }
    } 
    else
        retID = -1;

    return retID;
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
// }

ISR(TIMER4_OVF_vect)
{
  //Capture a "snapshot" of the encoder counts for later processing
  countsLeft = encoders.getCountsLeft();
  countsRight = encoders.getCountsRight();

  PIDController::readyToPID = 1;
}