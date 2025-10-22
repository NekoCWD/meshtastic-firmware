#include "configuration.h"

#if !MESHTASTIC_EXCLUDE_ENVIRONMENTAL_SENSOR && __has_include(<DHT.h>)
#ifdef DHT_PIN

#include "../mesh/generated/meshtastic/telemetry.pb.h"
#include "DHT.h"
#include "TelemetrySensor.h"

#include <DHT.h>

/*
 * TODO: Modify protobufs to add support for DHT (DHT11/DHT22) sensors
 */
DHTSensor::DHTSensor() : TelemetrySensor(meshtastic_TelemetrySensorType_AHT10, "DHT") {}

bool DHTSensor::initDevice(TwoWire *bus, ScanI2C::FoundDevice *dev) {
    LOG_INFO("DHT sensor init");
    dht.begin();
    if (isnan(dht.readHumidity())) {
        LOG_INFO("DHT sensor init failed (NaN value received)");
        nodeTelemetrySensorsMap[sensorType].first = 0;
    } else {
        LOG_INFO("DHT sensor successfully initialized");
        status = true;
        initialized = true;
    }
    return status;
}

bool DHTSensor::getMetrics(meshtastic_Telemetry *measurement)
{
    float humidity = dht.readHumidity();
    float temperature = dht.readTemperature();

    LOG_DEBUG("DHT sensor: %f°C %f%%", temperature, humidity);

    measurement->variant.environment_metrics.has_temperature = true;
    measurement->variant.environment_metrics.temperature = temperature;

    measurement->variant.environment_metrics.has_relative_humidity = true;
    measurement->variant.environment_metrics.relative_humidity = humidity;

    return true;
}

#endif
#endif
