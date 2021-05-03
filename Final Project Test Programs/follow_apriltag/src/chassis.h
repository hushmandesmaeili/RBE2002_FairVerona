#ifndef __CHASSIS_H
#define __CHASSIS_H

#include <Romi32U4.h>
#include "PIDcontroller.h"
#include <apriltags.h>

class Chassis
{
private:
    //parameters -- these will need to be updated after you do your experiments
    float wheel_track = 15.2; //cm
    float wheel_diam = 7.085; //cm
    float ticks_per_rotation = 1440; // from the datasheet

    //constants to control speed of the wheels for inverse kinematics
    const float kpD = 0.75;
    const float kpTheta = 12;

    //constants to control of speed of the wheels based on camera
    const float kp_distance = 2.7;
    const float kp_alignment = 0.1;

    //constant for camera offset from front of chassis
    const float CAMERA_OFFSET = 12.0;

    //current Pose
    float x = 50;
    float y = 0;
    float theta = PI / 2;

    //
    const int BUFFER_TARGET_POSE = 4;
    const int BUFFER_CAMERA_FOLLOWER = 2;

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

    //AprilTag data structure
    AprilTagDatum tag;

public:
    Chassis(void);
    void Init(void);
    void SetTargetSpeeds(float left, float right) { targetSpeedLeft = left; targetSpeedRight = right; }
    void SetTargetPosition(float xt, float yt) { x_target = xt; y_target = yt; }
    void UpdatePose(void);
    void UpdateSpeeds(void);
    void MoveToPoint(void);
    bool AreWeThere(void);
    void FollowAprilTag(float targetDistance);
    int DetectAprilTag();
};

#endif