#include <Romi32U4.h>

const int receiver = 12;

unsigned long currTime;
unsigned long prevTime;
const int INTERVAL = 50;

void setup() {
    Serial.begin(115200);
    pinMode(receiver, INPUT);
    prevTime = millis();
}

void loop() {
    currTime = millis();
    if (currTime - prevTime >= INTERVAL) {
        int output = digitalRead(receiver);
        Serial.print(output);
        Serial.print("\n");
        prevTime = millis();
    }
}