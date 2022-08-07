
#ifndef __SAMPLER_H
#define __SAMPLER_H
#include <Seeed_Arduino_ooFreeRTOS.h>
#include <Arduino.h>
#include "sensor.h"
#include "utils.h"


using namespace cpp_freertos;

class SamplerThread : public Thread
{
public:
    SamplerThread(Message &m1);

protected:
  virtual void Run();

    // while (true) {
    //   for (auto sensor : sensor_base::sensors) {
    //     Serial.printf("Sampling %s\n", sensor->get_name());
    //     Delay(Ticks::MsToTicks(10));
    // }
  //}

private:
  int DelayInMs;
  sensor_base *sensor;

private:
   struct sensor_data sdata;

   Message &sensorMail;

};


#endif