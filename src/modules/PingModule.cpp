#include "PingModule.h"
#include "MeshService.h"
#include <Arduino.h>

#include "MeshTypes.h"
#include "Router.h"
#include "meshtastic/portnums.pb.h"

#ifndef MODULE_PING_DELAY
#define MODULE_PING_DELAY 30000
#endif

PingModule::PingModule() : concurrency::OSThread("GenericThreadModule") {}

int32_t PingModule::runOnce()
{
  auto reply = router->allocForSending();

  reply->to = NODENUM_BROADCAST;
  reply->decoded.portnum = meshtastic_PortNum_TEXT_MESSAGE_APP;
  reply->want_ack = true;

  const char message[] = "Ping";

  reply->decoded.payload.size = strlen(message);
  memcpy(reply->decoded.payload.bytes, message, reply->decoded.payload.size);

  service->sendToMesh(reply, RX_SRC_LOCAL, true);

  return (MODULE_PING_DELAY);
}
