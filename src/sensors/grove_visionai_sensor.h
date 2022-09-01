#ifndef __GROVE_VISIONAI_SENSOR_H
#define __GROVE_VISIONAI_SENSOR_H

#include "Seeed_Arduino_GroveAI.h"
#include "SoftwareI2C.h"
#include "SysConfig.h"
#include "sensor.h"
#include <Seeed_Arduino_ooFreeRTOS.h>
#include <utils.h>

using namespace cpp_freertos;

#define VISIONAI_SDAPIN D1
#define VISIONAI_SCLPIN D0
#define MAX_DETECTION 10

class Visionai : public Thread {

  public:
    Visionai();
    int                     data[20];
    uint8_t                 dsize;
    bool                    status;
    std::vector<log_data>   ai_log;
    SoftwareI2C             softwarei2c;
    GroveAI                 ai = GroveAI(softwarei2c);

  protected:
    virtual void Run();
    void         pushlog(const char *msg);
};

class grove_visionai_sensor : public sensor_base {
  public:
    grove_visionai_sensor();
    void        init();
    const char *get_name();
    bool        read(struct sensor_data *data);

  private:
    const char *name = "visionai sensor"; /// buildin-light
    int         visionai_value[20];
    Visionai   *visionai;
};

#endif