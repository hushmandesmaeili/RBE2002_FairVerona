#pragma once

#include "Romi32U4.h"
#include "Romi32U4Motors.h"
#include "PIDcontroller.h"
#include "ourTimer.h"
#include "Romi32U4Encoders.h"
#include "LSM6.h"
#include "apriltags.h"

class Chassis{
    public:
        void setup();
        void loop();
        // void startWallFollow(); //sets flag that starts wallfollowing
        // void stopWallFollow();
        //velocity control is always enabled, only works when wall following not enabled
        void setMotorSpeeds(int left, int right); //speeds -75 to 75
        void SetTargetPosition(float xt, float yt, float th_t) { x_target = xt; y_target = yt; th_target = th_t; }
        void resetPose(void);
        void MoveToPoint(void);
        bool AtTargetPosition(void);
        void FollowAprilTag(float targetDistance);
        int DetectAprilTag();
        bool checkIfOnRamp(); //getter that returns true when robot is on ramp
        bool getHasCollided(); //true if robot has detected a collision at any point
        bool IsCalibrating(void);
        void GetXAverage(void);
        void updatePose(void);
        void updateSpeeds(void);
        // int16_t getCountsLeft(void);
        // int16_t getCountsRight(void);

        //current Pose
        float x = 0;
        float y = 0;
        float theta = 0; //in radians

        //enables
        bool wallFollowDirection = 1; //1 for forwards wall follow
        bool checkRampEnable = 0;
        bool wallFollowEnable = 0;
        bool updatePoseEnable = 0;
        bool detectCollisionEnable = 0;
        // bool updatePoseEnable = 0;
    private:
        bool manualSpeedsEnable = 1;
        bool hasCollided;

        void collisionDetect(void);
        bool UpdatePitch(void);
        float getPitchAng(void);
        void wallFollower(void);
        // void updateSpeeds(void);
        float getDistance();
        float rollingAverage(float arr[5]);
        // void updatePose(void);
        void checkRamp(void);

        //parameters -- these will need to be updated after you do your experiments
        float wheel_track = 15.2; //cm
        float wheel_diam = 7.085; //cm
        float ticks_per_rotation = 1440; // from the datasheet

        //actual speed
        int16_t speedLeft = 0;
        int16_t speedRight = 0;

        ///////////////////////////////
        /////////// variables /////////
        ///////////////////////////////

        Romi32U4Motors motors;
        float targetSpeedLeft = 0;
        float targetSpeedRight = 0;

        //constant max speed
        const float MAX_SPEED_LIMIT = 30;

        //current target
        float x_target = 0;
        float y_target = 0;
        float th_target = 0; //may or may not be specified

        //constants for buffers
        const int BUFFER_TARGET_POSE = 4;
        const int BUFFER_FINAL_HEADING = 1;
        const int BUFFER_CAMERA_FOLLOWER = 0.3;

        //constants to control speed of the wheels for inverse kinematics
        const float kpD = 0.75;
        const float kpTheta = 12;
        const float kpFinalHeading = 5;

        //constants to control of speed of the wheels based on camera
        const float kp_distance = 2.7;
        const float kp_alignment = 0.1;

        //constant for camera offset from front of chassis
        const float CAMERA_OFFSET = 12.0;

        //AprilTag data structure
        AprilTagDatum tag;

        const uint8_t sharpRead = 18; //pin for Sharp IR
        const uint8_t sharpRead2 = 22; //A4
        const float VREF = 5.0;
        float lastSharpSamples[5];
        uint16_t sampleCount = 0;
        // uint16_t adc_out;

        //constants for wall follow
        uint16_t targetDistance = 14; //in cm
        int16_t turnEffort = 0;
        int16_t targetSpeed = 15;
        
        //imu and ramp constants
        LSM6 imu;

        bool onRamp = 0; //true if on ramp
        float upperLimit = 0.15;
        float lowerLimit = 0.1; //limit when on ramp

        //be sure to set this -- it needs to match your "readyToPID" period
        uint32_t timestepMS = 16; // in ms

        
        float estimatedPitchAng, pitchLast;
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
};