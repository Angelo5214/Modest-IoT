#include "SoilTemperatureSensor.h"
#include <Arduino.h>

SoilTemperatureSensor::SoilTemperatureSensor(int analogGpioPin, float minimumCelsius,
                                             float maximumCelsius, EventHandler* parentHandler)
    : Sensor(analogGpioPin, parentHandler),
      minRange(minimumCelsius),
      maxRange(maximumCelsius),
      isOverRange(false),
      isUnderRange(false),
      temperature(0.0f) {
    pinMode(pin, INPUT);
}

void SoilTemperatureSensor::processEvent(Event& event) {
    if (event.identifier != MEASURE_DATA_REQUESTED_EVENT_IDENTIFIER) {
        event.identifier = PROPAGATION_SUPPRESSION_IDENTIFIER;
        return;
    }

    const int rawCount = analogRead(pin);
    temperature = SCALE_MINIMUM_CELSIUS
        + (static_cast<float>(rawCount) / ADC_FULL_SCALE_COUNTS)
        * (SCALE_MAXIMUM_CELSIUS - SCALE_MINIMUM_CELSIUS);

    if (temperature < SCALE_MINIMUM_CELSIUS) {
        temperature = SCALE_MINIMUM_CELSIUS;
    } else if (temperature > SCALE_MAXIMUM_CELSIUS) {
        temperature = SCALE_MAXIMUM_CELSIUS;
    }

    if ((temperature < minRange) && !isUnderRange) {
        isUnderRange = true;
        isOverRange = false;
        event = UNDER_EVENT;
    } else if ((temperature > maxRange) && !isOverRange) {
        isOverRange = true;
        isUnderRange = false;
        event = OVER_EVENT;
    } else if (temperature >= minRange && temperature <= maxRange && (isOverRange || isUnderRange)) {
        isOverRange = false;
        isUnderRange = false;
        event = NORMAL_EVENT;
    } else {
        event.identifier = PROPAGATION_SUPPRESSION_IDENTIFIER;
    }
}

float SoilTemperatureSensor::getTemperatureInCelsius() const {
    return temperature;
}