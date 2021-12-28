#pragma once

#include "Romi32U4.h"
#include "Romi32U4Motors.h"
#include "PIDcontroller.h"
#include "ourTimer.h"
#include "Romi32U4Encoders.h"
#include <LSM6.h>
#include "apriltags.h"

class Chassis{
    
    public:
        static Chassis& Instance(); //class method as the only access to the singleton object

        void setup();
        void loop();
        // void startWallFollow(); //sets flag that starts wallfollowing
        // void stopWallFollow();
        //velocity control is always enabled, only works when wall following not enabled
        void setMotorSpeeds(int left, int right); //speeds -75 to 75
        void SetTargetPosition(float xt, float yt, float th_t) { x_target = xt; y_target = yt; th_target = th_t; }
        void resetPose(void);
        void MoveToPoint(void);
        bool AtTargetPosition();
        bool AtTargetPosition(float buffer_xy, float buffer_theta);
        void driveCircle(float radius, float speed);
        void FollowAprilTag(float targetDistance);
        int DetectAprilTag();
        bool checkIfOnRamp(); //getter that returns true when robot is on ramp
        bool getHasCollided(); //true if robot has detected a collision at any point
        bool IsCalibrating(void);
        void GetXAverage(void);
        void updatePose(void);
        void updateSpeeds(void);
        float getDistance();
        void FollowAprilTag(float targetDistance, float maxSpeed);
        float getDistanceCamera();
        float getPitchAng(void);
        // int16_t getCountsLeft(void);
        // int16_t getCountsRight(void);

        //current Pose
        float x = 0;
        float y = 0;
        float theta = 0; //in radians
        float th_target = 0; //may or may not be specified

        //enable;
        bool wallFollowForwardEnable = 0;  //1 for forwards wall follow enabled
        bool wallFollowBackwardEnable = 0;  //1 for backwards wall follow enabled
        bool checkRampEnable = 0;
        // bool wallFollowEnable = 0;
        bool updatePoseEnable = 0;
        // bool updatePoseEnable = 0;

        float estimatedPitchAng;

        //standard buffers for moving to point
        const float BUFFER_TARGET_POSE_STD = 4;
        const float BUFFER_FINAL_HEADING_STD = 0.1;
    private:
        bool manualSpeedsEnable = 1;
        bool hasCollided;

        static Chassis* pInstance; //pointer to only instance of the class
        Chassis(); //constructor

        void collisionDetect(void);
        bool UpdatePitch(void);
        void wallFollower(void);
        // void updateSpeeds(void);
        // float getDistance();
        float rollingAverage(float arr[5]);
        // void updatePose(void);
        void checkRamp(void);

        //parameters -- these will need to be updated after you do your experiments
        float wheel_track = 14.3; //cm
        float wheel_diam = 7.085; //cm
        float ticks_per_rotation = 1440; // from the datasheet
        const float ticksToCMSpeed = 0.96607; 

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
        // float th_target = 0; //may or may not be specified

        //constants for buffers
        const float BUFFER_CAMERA_FOLLOWER = 0.0;

        //constants to control speed of the wheels for inverse kinematics
        const float kpD = 0.75;
        const float kpTheta = 12;
        const float kpFinalHeading = 2;

        //constants to control of speed of the wheels based on camera
        const float kp_distance = 2.7;
        const float kp_alignment = 0.2;

        //constant for camera offset from front of chassis
        const float CAMERA_TIMEOUT= 500;
        const float CAMERA_OFFSET = 12.0;
        const float TAG_OFFSET = 7.5;

        //AprilTag data structure
        AprilTagDatum tag;

        const uint8_t sharpRead = 18; //A0 pin target distance -- for front Sharp IR SAM
        const uint8_t sharpRead2 = 22; //A4 -- target distance 15cm for rear IR FREUD
        const float VREF = 5.0;
        float lastSharpSamples[5];
        uint16_t sampleCount = 0;
        // uint16_t adc_out;

        //constants for wall follow
        // uint16_t targetDistance = 15; //in cm
        uint16_t targetDistanceFW = 15;
        uint16_t targetDistanceBW = 15;
        int16_t turnEffort = 0;
        int16_t targetSpeed = 10;
        
        //imu and ramp constants
        LSM6 imu;

        bool onRamp = 0; //true if on ramp
        float upperLimit = 0.15;
        float lowerLimit = 0.1; //limit when on ramp

        //be sure to set this -- it needs to match your "readyToPID" period
        uint32_t timestepMS = 16; // in ms

        
        float pitchLast;
        const float  dataRateSec = 0.077;
        const float  sensitivity = 0.035;  //sensitivity of gyro in mdps/LSB
        float senseRad = (sensitivity * PI)/(180.); // rads per second per LSB
        const float kappa = 0.85;

        float accXoffset = 0;
        float accZoffset = 0;
        int sampleSize = 0; 
        bool isCalibrating = false;

        float gyroBias = 0;
        float Bias = 0;
        const float E = 0.01;
};
