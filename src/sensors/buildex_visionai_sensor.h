#ifndef __BUILDEX_VISIONAI_SENSOR_H
#define __BUILDEX_VISIONAI_SENSOR_H

#include "Seeed_Arduino_GroveAI.h"
#include "SoftwareI2C.h"
#include "SysConfig.h"
#include "sensor.h"
#include <Arduino.h>
#include <Seeed_Arduino_ooFreeRTOS.h>
#include <utils.h>

using namespace cpp_freertos;

#define VISIONAI_SDAPIN D1
#define VISIONAI_SCLPIN D0
#define MAX_DETECTION 3

class buildex_visionai_sensor : public sensor_base {
  public:
    buildex_visionai_sensor();
    void        init();
    const char *get_name();
    bool        read(struct sensor_data *data);

  private:
    const char *name = "visionai sensor"; /// buildin-light
    int         visionai_value[3];
    uint8_t     state;
    SoftwareI2C softwarei2c;
    GroveAI     ai = GroveAI(softwarei2c);
};

#endif