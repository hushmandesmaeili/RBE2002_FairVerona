#include <Arduino.h>
#include <Romi32U4.h>
#include <Timer.h>

const int INTERVAL = 1000;

Timer timerBlink = Timer(INTERVAL);

void setup() {
  // put your setup code here, to run once:
  pinMode(LED_BUILTIN, OUTPUT);
  timerBlink.reset();
}

void loop() {
  // put your main code here, to run repeatedly:
  if (timerBlink.isExpired()) {
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
  }
}