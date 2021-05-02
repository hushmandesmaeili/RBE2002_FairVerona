#ifndef __APRILTAGS_H
#define __APRILTAGS_H

#include <Arduino.h>

/*
 * AprilTagDatum is the data received from the camera 
*/
struct AprilTagDatum { uint16_t header_aa55, header_aa56, checksum, id = 0, cx, cy, w, h, rot; };

const float actualWidth = 6.1; //actual width in cm
const float calDistance = 20.0; //calibration distance in cm
const float calWidth = 45; //in pseudo-pixels

const float centerCX = 80; //center pixel in x direction

uint8_t getTagCount(void);
bool readTag(AprilTagDatum *tag);
float getDistanceCam(uint16_t tagW);
float getDeltaCXCam(uint16_t tagCX);

#endif