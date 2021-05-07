#ifndef __COMP_FILTER_H
#define __COMP_FILTER_H

#include <Wire.h>
#include <Romi32U4.h>

class TapDetector
{
 private:
  LSM6 imu;
//   Romi32U4Motors motors;

public: 
  void Init(void);
  bool CheckTap(void);
};

#endif