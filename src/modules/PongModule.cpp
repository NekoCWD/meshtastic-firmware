#include "PongModule.h"
#include "MeshService.h"
#include "MeshTypes.h"
#include "meshtastic/mesh.pb.h"
#include "meshtastic/portnums.pb.h"

#include <cstdio>
#include <cstring>

ProcessMessage PongModule::handleReceived(const meshtastic_MeshPacket &mp)
{
#ifdef MODULE_PONG_IGNORE_MQTT
  if (mp.via_mqtt)
    return ProcessMessage::CONTINUE;
#endif
  meshtastic_MeshPacket* reply = allocDataPacket();
  char *message = new char[60];
  message[0]='\0';
  reply->channel = mp.channel;
  if (isBroadcast(mp.to))
    reply->to = mp.to;

  else
    reply->to = mp.from;
  reply->want_ack = true;

  if (mp.via_mqtt)
      sprintf(message, "MQTT ");
  sprintf(message + strlen(message), "Pong to !%x\n",mp.from);

  if (mp.hop_start == mp.hop_limit)
    // Direct ping, SNR/RSSI can be helpful
    if(mp.rx_rssi != 0 && mp.rx_snr != 0)
        sprintf(message + strlen(message), "R:%d S:%.2f", mp.rx_rssi, mp.rx_snr);
    else
        sprintf(message + strlen(message), "Zero hop & zero signal");
  else
    // Ping was obtained via mesh, SNR/RSSI can't be helpful, but hop count can
    sprintf(message + strlen(message), "Hops:%d/%d", mp.hop_start - mp.hop_limit, mp.hop_start);

  reply->decoded.payload.size = strlen(message);
  memcpy(reply->decoded.payload.bytes, message, reply->decoded.payload.size);

  service->sendToMesh(reply, RX_SRC_LOCAL, true);
  delete[] message;

  return ProcessMessage::CONTINUE;
}

bool PongModule::wantPacket(const meshtastic_MeshPacket *p)
{
  if (!MeshService::isTextPayload(p) ||
      p->decoded.portnum != meshtastic_PortNum_TEXT_MESSAGE_APP ||
      p->from == 0) {
      return false;
  }

  if (strcasecmp("Ping", (const char*)p->decoded.payload.bytes) == 0) {
    return true;
  }

  return false;
}
