#ifndef __VISIONAI_H__
#define __VISIONAI_H__
#include "sensor.h"
#include "utils.h"
#include "Wire.h"
#include "Seeed_Arduino_GroveAI.h"

#include <Seeed_Arduino_ooFreeRTOS.h>

using namespace cpp_freertos;

class Vision : public Thread {

  public:
    Vision();
    int     data[20];
    uint8_t dsize;
    bool    status;

  protected:
    virtual void Run();
};

class VisionSensor : public sensor_base
{
public:
    VisionSensor();
    void init();
    bool read(struct sensor_data *data);
    const char *get_name();
private:
    const char *name = "Vision_AI";
    bool status;
    Vision       *vision;
};

#endif // __VISIONAI_H__