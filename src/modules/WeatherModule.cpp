#include "WeatherModule.h"
#include "MeshModule.h"
#include "MeshService.h"
#include "MeshTypes.h"
#include "meshtastic/mesh.pb.h"
#include "meshtastic/portnums.pb.h"
#include "modules/Telemetry/EnvironmentTelemetry.h"

#include <cstdio>
#include <cstring>

ProcessMessage WeatherModule::handleReceived(const meshtastic_MeshPacket &mp) {
  if (environmentTelemetryModule == nullptr) {
    return ProcessMessage::CONTINUE;
  }

  meshtastic_Telemetry m = meshtastic_Telemetry_init_zero;
  if (!environmentTelemetryModule->getEnvironmentTelemetry(&m)) {
    return ProcessMessage::CONTINUE;
  }

  char *message = new char[60];

  /*
   * TODO: Add other metrics
   */
  if(m.variant.environment_metrics.has_temperature)
    sprintf(message, "T: %.1f°C\n", m.variant.environment_metrics.temperature);
  if(m.variant.environment_metrics.has_relative_humidity)
    sprintf(message+strlen(message), "H: %.1f%%\n", m.variant.environment_metrics.relative_humidity);
  if(m.variant.environment_metrics.has_barometric_pressure)
    sprintf(message+strlen(message), "P: %f.1hPA\n", m.variant.environment_metrics.barometric_pressure);
  if (message[strlen(message) - 1] == '\n')
    message[strlen(message) - 1] = '\0';
  meshtastic_MeshPacket* reply = allocDataPacket();
  reply->channel = mp.channel;
  if (isBroadcast(mp.to))
    reply->to = mp.to;
  else
    reply->to = mp.from;
  reply->want_ack = true;

  reply->decoded.payload.size = strlen(message);
  memcpy(reply->decoded.payload.bytes, message, reply->decoded.payload.size);

  service->sendToMesh(reply, RX_SRC_LOCAL, true);
  delete[] message;

  return ProcessMessage::CONTINUE;
}

bool WeatherModule::wantPacket(const meshtastic_MeshPacket *p)
{
  if (!MeshService::isTextPayload(p) ||
      p->decoded.portnum != meshtastic_PortNum_TEXT_MESSAGE_APP ||
      p->from == 0) {
      return false;
  }

  if (strcasecmp("Weather", (const char*)p->decoded.payload.bytes) == 0) {
    return true;
  }
  if (strcasecmp("Погода", (const char*)p->decoded.payload.bytes) == 0) {
    return true;
  }

  return false;
}