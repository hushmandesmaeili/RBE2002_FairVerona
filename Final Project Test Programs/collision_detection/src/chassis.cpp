
#include "Chassis.h"
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

bool Chassis::Collided(void){

    return 1;
}

void Chassis::ImuVal(void){
    if(imu.getStatus() & 0x01) {
imu.read();
Serial.print(imu.a.x);
Serial.print("\n");
    }
}
