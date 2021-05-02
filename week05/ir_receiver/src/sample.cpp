#include <Wire.h>
#include <LSM6.h>
#include <math.h>

//sensitivty of accelerometer at FS ±4, in g/LSB
const float sensitivity = 0.000122;

//conversion factor i.e. in m/s^2 per g
const float gToMS2 = 9.81;

// Reads the y-axis accelerometer channel, converts reading to m/s2 and returns value
float readAccY(void)
{
  Wire.beginTransmission(0b1101011);
  // automatic increment of register address is enabled by default (IF_INC in CTRL3_C)
  Wire.write(0x2A);
  Wire.endTransmission();
  Wire.requestFrom(0b1101011, (uint8_t)2);

  while (Wire.available() < 2)
  {
  }

  uint8_t yla = Wire.read();
  uint8_t yha = Wire.read();

  // combine high and low bytes
  int16_t accReadingY = (int16_t)(yha << 8 | yla);

  float accY = accReadingY * sensitivity * gToMS2;

  return accY;

}

