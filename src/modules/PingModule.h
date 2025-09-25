#pragma once

#include "concurrency/OSThread.h"
#include <Arduino.h>

class PingModule : private concurrency::OSThread
{
  public:
    PingModule();

  protected:
    virtual int32_t runOnce() override;
};
