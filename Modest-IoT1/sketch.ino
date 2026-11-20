#include <Arduino.h>
#include <ModestIoT.h>
#include "GreenPulseDevice.h"

GreenPulseDevice device;

void setup() {
    Serial.begin(115200);
    device.begin();
    Serial.println("GreenPulse Technologies");
    Serial.println("GreenPulse Smart Irrigation");
    Serial.println("Solano Armas Angelo");
}

void loop() {
    delay(1000);
}