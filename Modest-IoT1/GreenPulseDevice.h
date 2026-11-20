#ifndef GREEN_PULSE_DEVICE_H
#define GREEN_PULSE_DEVICE_H

#include <ModestIoT.h>
#include "SoilTemperatureSensor.h"
#include "SoilTemperatureRecord.h"

class GreenPulseWiFiDriver : public WiFiConnectivityDriver {
public:
    GreenPulseWiFiDriver(const char* ssid, const char* password)
        : WiFiConnectivityDriver(ssid, password) {}

    bool transmit(const char* target, const char* data) override {
        (void)target;
        (void)data;
        return canTransmit();
    }
};

class GreenPulseDevice : public Device {
private:
    SoilTemperatureSensor soilTemperatureSensor;
    Led heatingLed;
    GreenPulseWiFiDriver wifiDriver;
    mutable HttpGatewayClient httpClient;

    void sendDataRecord(const char* eventName, const char* heatingState);

public:
    static constexpr int HEATING_RELAY_PIN = 26;
    static constexpr float LOW_TEMPERATURE_CELSIUS = 20.0f;
    static constexpr float HIGH_TEMPERATURE_CELSIUS = 35.0f;
    static constexpr unsigned long SAMPLING_INTERVAL_IN_MILLISECONDS = 1000;

    static constexpr const char* WIFI_SSID = "Wokwi-GUEST";
    static constexpr const char* WIFI_PASSWORD = "";
    static constexpr const char* ENDPOINT_URL =
        "https://iot-edge-202610-1asi0572-sandbox.free.beeceptor.com/api/v1/data-records";
        
    static constexpr const char* EVENT_UNDER = "under";
    static constexpr const char* EVENT_OVER = "over";
    static constexpr const char* EVENT_NORMAL = "normal";
    static constexpr const char* HEATING_ON = "on";
    static constexpr const char* HEATING_OFF = "off";

    GreenPulseDevice();
    void begin();
    void on(Event event) override;
    void processQueuedTelemetryData(const TelemetryPackage* rawQueueItemPayload) const override;
};

#endif