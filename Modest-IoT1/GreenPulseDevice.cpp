#include "GreenPulseDevice.h"
#include <Arduino.h>

GreenPulseDevice::GreenPulseDevice()
    : Device(SAMPLING_INTERVAL_IN_MILLISECONDS),
      soilTemperatureSensor(SoilTemperatureSensor::SOIL_TEMPERATURE_PIN,
                            LOW_TEMPERATURE_CELSIUS,
                            HIGH_TEMPERATURE_CELSIUS,
                            this),
      heatingLed(HEATING_RELAY_PIN, false, this),
      wifiDriver(WIFI_SSID, WIFI_PASSWORD),
      httpClient(wifiDriver, ENDPOINT_URL) {}

void GreenPulseDevice::begin() {
    wifiDriver.connect();
    const unsigned long startedAt = millis();
    while (!wifiDriver.isConnected() && (millis() - startedAt) < 8000) {
        delay(250);
        wifiDriver.connect();
    }

    if (wifiDriver.isConnected()) {
        Serial.println("WiFi conectado");
    } else {
        Serial.println("WiFi pendiente");
    }

    appendSensorToScheduler(
        &soilTemperatureSensor,
        Sensor::MEASURE_DATA_REQUESTED_EVENT_IDENTIFIER);
    initializeAsynchronousEngine();
}

void GreenPulseDevice::sendDataRecord(const char* eventName, const char* heatingState) {
    TelemetryPackage* record = new SoilTemperatureRecord(
        eventName,
        soilTemperatureSensor.getTemperatureInCelsius(),
        heatingState);

    if (!enqueueTelemetryPayload(&record)) {
        delete record;
        Serial.println("Cola llena");
    }
}

void GreenPulseDevice::on(Event event) {
    if (event == SoilTemperatureSensor::UNDER_EVENT) {
        heatingLed.handle(Led::TURN_ON_COMMAND);
        Serial.println("UNDER");
        Serial.println("Calefaccion: ON");
        sendDataRecord(EVENT_UNDER, HEATING_ON);
    } else if (event == SoilTemperatureSensor::OVER_EVENT) {
        heatingLed.handle(Led::TURN_OFF_COMMAND);
        Serial.println("OVER");
        Serial.println("Calefaccion: OFF");
        sendDataRecord(EVENT_OVER, HEATING_OFF);
    } else if (event == SoilTemperatureSensor::NORMAL_EVENT) {
        heatingLed.handle(Led::TURN_OFF_COMMAND);
        Serial.println("NORMAL");
        Serial.println("Calefaccion: OFF");
        sendDataRecord(EVENT_NORMAL, HEATING_OFF);
    }
}

void GreenPulseDevice::processQueuedTelemetryData(const TelemetryPackage* rawQueueItemPayload) const {
    if (rawQueueItemPayload == nullptr) {
        return;
    }

    const bool accepted = httpClient.sendTelemetryRecord(*rawQueueItemPayload);
    Serial.println(accepted ? "HTTP POST accepted." : "HTTP POST pending.");
}