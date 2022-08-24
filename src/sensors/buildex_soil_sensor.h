#ifndef __BUILDEX_SOIL_SENSOR_H
#define __BUILDEX_SOIL_SENSOR_H

#include "sensor.h"
#include "utils.h"
#include "Arduino.h"

#define SOILPIN A0

class buildex_soil_sensor : public sensor_base {
  public:
    buildex_soil_sensor();
    void        init();
    const char *get_name();
    bool        read(struct sensor_data *data);

  private:
    const char *name = "soil sensor"; /// buildin-light
    int         soil_value;
};

#endif