#ifndef SOIL_TEMPERATURE_RECORD_H
#define SOIL_TEMPERATURE_RECORD_H

#include <ModestIoT.h>

class SoilTemperatureRecord : public TelemetryPackage {
public:
    static constexpr const char* KEY_EVENT = "event";
    static constexpr const char* KEY_TEMPERATURE = "temperature";
    static constexpr const char* KEY_HEATING = "heating";

    const char* eventName;
    float temperatureCelsius;
    const char* heatingState;

    SoilTemperatureRecord(const char* eventName, float temperatureCelsius, const char* heatingState)
        : eventName(eventName),
          temperatureCelsius(temperatureCelsius),
          heatingState(heatingState) {}

    void serialize(JsonDocument& serializationDestination) const override {
        serializationDestination[KEY_EVENT] = eventName;
        serializationDestination[KEY_TEMPERATURE] = temperatureCelsius;
        serializationDestination[KEY_HEATING] = heatingState;
    }
};

#endif