#include <Arduino.h>

//for scheduling pings
uint32_t lastPing = 0;
const uint32_t PING_INTERVAL = 50; //ms

//this may be most any pin, connect the pin to read on the sensor
const uint8_t readPin = 18;

const float VREF = 5.0;

//count to keep track of sample number
uint8_t sampleCount = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  while(!Serial) {} //you must open the Serial Monitor to get past this step!
  sampleCount = 0;

  pinMode(readPin, INPUT);
  
  lastPing = millis();
}

void loop() {
  // put your main code here, to run repeatedly:
  uint32_t currTime = millis();
  if((currTime - lastPing) >= PING_INTERVAL && sampleCount < 200)
  {
    uint16_t adc_out = analogRead(readPin);
    float voltage_out = ((float) adc_out * VREF) / 1023;
    sampleCount++;
    Serial.print(sampleCount);
    Serial.print('\t');
    Serial.print(adc_out);
    Serial.print('\t');
    Serial.print(voltage_out);
    Serial.print('\n');
    lastPing = currTime;
  }
}