#pragma once
#include "SinglePortModule.h"
#include "meshtastic/portnums.pb.h"

/**
 * A simple module that replies with environmental metrics to "Weather" message.
 */
class WeatherModule : public SinglePortModule
{
  public:
    WeatherModule() : SinglePortModule("weather", meshtastic_PortNum_TEXT_MESSAGE_APP) {}

  protected:
    virtual ProcessMessage handleReceived(const meshtastic_MeshPacket &mp) override;
    virtual bool wantPacket(const meshtastic_MeshPacket *p) override;
};
