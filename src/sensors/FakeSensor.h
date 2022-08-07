#ifndef __FAKESENSOR_H__
#define __FAKESENSOR_H__

#include "sensor.h"
#include "utils.h"

#include <Seeed_Arduino_ooFreeRTOS.h>

using namespace cpp_freertos;

class Fake : public Thread {

  public:
    Fake();
    int     data[10];
    uint8_t dsize;
    bool    status;

  protected:
    virtual void Run();
};

class FakeSensor : public sensor_base {
  public:
    FakeSensor();
    void        init();
    bool        read(struct sensor_data *data);
    const char *get_name();

  private:
    const char *name = "FakeSensor";
    bool        status;
    Fake       *fake;
};

#endif // __FAKESENSOR_H__