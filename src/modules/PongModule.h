#pragma once
#include "SinglePortModule.h"
#include "meshtastic/portnums.pb.h"

/**
 * A simple module that replies with "Pong ..." to "Ping" message.
 */
class PongModule : public SinglePortModule
{
  public:
    PongModule() : SinglePortModule("pong", meshtastic_PortNum_TEXT_MESSAGE_APP) {}

  protected:
    virtual ProcessMessage handleReceived(const meshtastic_MeshPacket &mp) override;
    virtual bool wantPacket(const meshtastic_MeshPacket *p) override;
};
