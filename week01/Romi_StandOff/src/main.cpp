#include <Arduino.h>
#include <Romi32U4.h>

// Function prototypes
float getDistance();
float RollingAverage(float arr[10]);

//for scheduling pings
uint32_t lastPing = 0;
const uint32_t PING_INTERVAL = 50; //ms

//this may be most any pin, connect the pin to read on the sensor
const uint8_t readPin = 18;

const float VREF = 5.0;

//count to keep track of sample number
uint16_t sampleCount = 0;

Romi32U4Motors motors;

//array to store last five samples
float lastSamples[10];

const float targetDistance = 20.0;
const float Kp = 7.0;
const float Ki = 0.0;
float sumOfErrors = 0.01;

void setup() {
  Serial.begin(115200);
  // while(!Serial) {} //you must open the Serial Monitor to get past this step!
  // sampleCount = 0;

  pinMode(readPin, INPUT);
  
  lastPing = millis();
}

void loop() {

  uint32_t currTime = millis();
  if((currTime - lastPing) >= PING_INTERVAL) {
    sampleCount++;
    lastSamples[sampleCount % 10] = getDistance();
    float avg_distance = RollingAverage(lastSamples);
    float error = avg_distance - targetDistance;
    sumOfErrors += error;
    int effort = error * Kp + sumOfErrors * Ki;
    motors.setEfforts(effort, effort);
    Serial.print(avg_distance);
    Serial.print('\n');
    lastPing = currTime;
  }
}

float getDistance() {
  uint16_t adc_out = analogRead(readPin);
  float voltage_out = ((float) adc_out * VREF) / 1023;

  // convert from voltage to distance
  // adapt to your own derived equation
  float distance = 14.2 / (voltage_out - 0.372);

  return distance;
}

/*
 * Returns the rolling average of array passed as argument
 */
float RollingAverage(float arr[10]) 
{
  float avg = 0;
  float sum = 0;
  
  int i;
  for (i = 0; i < 10; i++)
    sum += arr[i];

  //if condition to check if first 10 samples to adjust denominator in average calculation
  if (sampleCount > 10)
    avg = sum / 10;
  else
    avg = sum / sampleCount;

  return avg;
}