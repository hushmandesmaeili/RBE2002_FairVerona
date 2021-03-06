#ifndef __CHASSIS_H
#define __CHASSIS_H

#include <Romi32U4.h>
#include "PIDcontroller.h"

class Chassis
{
private:
    //parameters -- these will need to be updated after you do your experiments
    float wheel_track = 15.2; //cm
    float wheel_diam = 7.085; //cm
    float ticks_per_rotation = 1440; // from the datasheet

    // constants to control speed of the wheels for inverse kinematics
    float kpD = 0.75;
    float kpTheta = 12;

    //current Pose
    float x = 0;
    float y = 0;
    float theta = 0;

    //
    const int BUFFER = 2;

    //current target
    float x_target = 0;
    float y_target = 0;
    float th_target = 0; //may or may not be specified

    //target wheels speeds
    float targetSpeedLeft = 0;
    float targetSpeedRight = 0;

    //for calculating speeds
    int16_t prevEncLeft = 0;
    int16_t prevEncRight = 0;

    //wheel speed controllers
    PIDController leftMotorController; 
    PIDController rightMotorController; 

    //actual speed
    int16_t speedLeft = 0;
    int16_t speedRight = 0;

    //be sure to set this -- it needs to match your "readyToPID" period
    uint32_t timestepMS = 16; // in ms

    
    float estimatedPitchAng;
    const float  dataRateSec = 0.077;
    const float  sensitivity = 35;  //sensitivity of gyro in mdps/LSB
    float senseRad = (sensitivity * PI)/(180000); // rads per second per LSB
    const float kappa = 0.5;

    float accXoffset = 0;
    float accZoffset = 0;
    int sampleSize = 0; 
    bool isCalibrating = false;

    float gyroBias = 0;
    float Bias = 0;
    const float E = 0.01;


public:
    Chassis(void);
    void Init(void);
    void SetTargetSpeeds(float left, float right) { targetSpeedLeft = left; targetSpeedRight = right; }
    void SetTargetPosition(float xt, float yt) { x_target = xt; y_target = yt; }
    void UpdatePose(void);
    void UpdateSpeeds(void);
    void MoveToPoint(void);
    bool AreWeThere(void);
    bool UpdatePitch(void);
    void GetXAverage(void);
    bool IsCalibrating(void);
    float getPitchAng(void);
    Romi32U4Motors motors;

};

#endif