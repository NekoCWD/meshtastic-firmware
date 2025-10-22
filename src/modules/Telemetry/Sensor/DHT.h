/*
 * DHT11/DHT22 sensor driver
 * For now there are only support for a single sensor
 * To use this driver, define DHT_PIN and DHT_TYPE (DHT11/DHT22(default to DHT22))
 */

#include "configuration.h"

#if !MESHTASTIC_EXCLUDE_ENVIRONMENTAL_SENSOR && __has_include(<DHT.h>)
#ifdef DHT_PIN
#ifndef DHT_TYPE
#define DHT_TYPE DHT22
#endif

#include "../mesh/generated/meshtastic/telemetry.pb.h"
#include "TelemetrySensor.h"
#include <DHT.h>

class DHTSensor : public TelemetrySensor
{
  private:
    DHT dht = DHT(DHT_PIN, DHT_TYPE);

  public:
    DHTSensor();
    virtual bool getMetrics(meshtastic_Telemetry *measurement) override;
    virtual bool initDevice(TwoWire *bus, ScanI2C::FoundDevice *dev) override;
};

#endif
#endif

