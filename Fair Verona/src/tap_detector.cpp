#include "tap_detector.h"

void TapDetector::Init(void)
{ 
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

    imu.setFullScaleGyro(LSM6::GYRO_FS1000);
    imu.setGyroDataOutputRate(LSM6::ODR104);

    imu.setFullScaleAcc(LSM6::ACC_FS2);
    imu.setAccDataOutputRate(LSM6::ODR104);

    imu.writeReg(0x59, 0x04); 
    imu.writeReg(0x58, 0x0C);

    //FS2 and 0x04 threshold work for speed 10 and 25 

    // Serial.print("Register 58: ");
    // Serial.print("\t");
    // Serial.print(imu.readReg(0x58));
    // Serial.print("\t");
    // Serial.print("Register 59: ");
    // Serial.print("\t");
    // Serial.print(imu.readReg(0x59));

//  motors.setEfforts(50,50);
}

bool TapDetector::CheckTap(void)
{
  
    uint8_t tapReg = imu.readReg(0x1C);
    if(tapReg & 0x06)
    {
        return true;
    } else
        return false;
} 