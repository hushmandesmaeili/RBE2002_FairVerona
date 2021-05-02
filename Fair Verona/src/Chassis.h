#pragma once

#include "Romi32U4.h"
#include "Romi32U4Motors.h"
#include "PIDcontroller.h"
#include "ourTimer.h"
#include "Romi32U4Encoders.h"
#include "LSM6.h"

class Chassis{
    public:
        void setup();
        void loop();
        void startWallFollow(); //sets flag that starts wallfollowing
        void stopWallFollow();
        //velocity control is always enabled, only works when wall following not enabled
        void setMotorSpeeds(int left, int right); //speeds -75 to 75
        bool checkIfOnRamp(); //getter that returns true when robot is on ramp
        bool IsCalibrating(void);
        void GetXAverage(void);
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
    private:
        bool manualSpeedsEnable = 1;

        bool UpdatePitch(void);
        float getPitchAng(void);
        void wallFollower(void);
        void updateSpeeds(void);
        float getDistance();
        float rollingAverage(float arr[5]);
        void updatePose(void);
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

        const uint8_t sharpRead = 18; //pin for Sharp IR
        const float VREF = 5.0;
        float lastSharpSamples[5];
        uint16_t sampleCount = 0;

        //constants for wall follow
        uint16_t targetDistance = 25;
        int16_t turnEffort = 0;
        int16_t targetSpeed = 15;

        //imu and ramp constants
        LSM6 imu;

        bool onRamp = 0; //true if on ramp
        float upperLimit = 0.1;
        float lowerLimit = 0.05;

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
};