
#ifndef __SAMPLER_H
#define __SAMPLER_H
#include "SysConfig.h"
#include "WiFiThread.h"
#include "sensor.h"
#include "utils.h"
#include <Arduino.h>
#include <Seeed_Arduino_ooFreeRTOS.h>

using namespace cpp_freertos;

class SamplerThread : public Thread {
  public:
    SamplerThread(SysConfig &config, Message &m1);

  protected:
    virtual void Run();

  private:
    int          DelayInMs;
    sensor_base *sensor;

  private:
    struct sensor_data sdata;

    Message   &sensorMail;
    SysConfig &cfg;

  private:
    WiFiThread *wifi;
};

#endif