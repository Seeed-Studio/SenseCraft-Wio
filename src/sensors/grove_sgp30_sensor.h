#ifndef __GROVE_SGP30_SENSOR_H
#define __GROVE_SGP30_SENSOR_H

#include "SparkFun_SGP30_Arduino_Library.h"
#include "sensor.h"
#include "utils.h"
#include "SoftwareI2C.h"

#define SGP30_SDAPIN D1
#define SGP30_SCLPIN D0

class grove_sgp30_sensor : public sensor_base {
  public:
    grove_sgp30_sensor();
    void        init();
    const char *get_name();
    bool        read(struct sensor_data *data);
    bool        is_connected;

  private:
    const char *name = "Gas"; 
    const char *data_unit = "ppm,ppb";
    int         sgp30_value[2];
    SGP30       sgp30;
};

#endif