#ifndef SOIL_TEMPERATURE_SENSOR_H
#define SOIL_TEMPERATURE_SENSOR_H

#include <ModestIoT.h>

class SoilTemperatureSensor : public Sensor {
private:
    float minRange;
    float maxRange;
    bool isOverRange;
    bool isUnderRange;
    float temperature;

    static constexpr int ADC_FULL_SCALE_COUNTS = 4095;
    static constexpr float SCALE_MINIMUM_CELSIUS = 0.0f;
    static constexpr float SCALE_MAXIMUM_CELSIUS = 100.0f;
    static constexpr int PROPAGATION_SUPPRESSION_IDENTIFIER = -1;

protected:
    void processEvent(Event& event) override;

public:
    static constexpr int SOIL_TEMPERATURE_PIN = 34;

    static constexpr int UNDER_EVENT_ID = 0;
    static constexpr Event UNDER_EVENT{UNDER_EVENT_ID};
    static constexpr int NORMAL_EVENT_ID = 1;
    static constexpr Event NORMAL_EVENT{NORMAL_EVENT_ID};
    static constexpr int OVER_EVENT_ID = 2;
    static constexpr Event OVER_EVENT{OVER_EVENT_ID};

    SoilTemperatureSensor(int analogGpioPin, float minimumCelsius, float maximumCelsius,
                          EventHandler* parentHandler = nullptr);
    float getTemperatureInCelsius() const;
};

#endif