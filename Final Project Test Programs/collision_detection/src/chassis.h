#ifndef __CHASSIS_H
#define __CHASSIS_H

#include <Romi32U4.h>



class Chassis
{
private:
    LSM6 imu;
    bool isCalibrating = false;
    int sampleSize = 0;
    float accXoffset = 0;
public:
    // Chassis(void);
    // void Init(void);
    // void SetTargetSpeeds(float left, float right) { targetSpeedLeft = left; targetSpeedRight = right; }
    // void SetTargetPosition(float xt, float yt) { x_target = xt; y_target = yt; }
    // void UpdatePose(void);
    // void UpdateSpeeds(void);
    // void MoveToPoint(void);
    // bool AreWeThere(void);
    void GetXAverage(void);
    bool IsCalibrating(void);
    bool Collided(void);
    void ImuVal(void);

};

#endif