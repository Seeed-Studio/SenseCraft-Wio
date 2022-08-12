
#ifndef __SAMPLER_H
#define __SAMPLER_H
#include "SysConfig.h"
#include "WiFiThread.h"
#include "LoRaThread.h"
#include "ui.h"
#include "sensor.h"
#include "utils.h"
#include <Arduino.h>
#include <Seeed_Arduino_ooFreeRTOS.h>

using namespace cpp_freertos;

class SamplerThread : public Thread {
  public:
    SamplerThread(SysConfig &config, UI &ui);

  protected:
    virtual void Run();

  private:
    int          DelayInMs;
    sensor_base *sensor;

  private:
    struct sensor_data sdata;

    UI   &display;
    SysConfig &cfg;

  private:
    WiFiThread *wifi;
    LoRaThread *lora;
};

#endif