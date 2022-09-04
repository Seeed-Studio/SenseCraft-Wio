#ifndef __GROVE_SGP30_SENSOR_H
#define __GROVE_SGP30_SENSOR_H

#include "SparkFun_SGP30_Arduino_Library.h"
#include "sensor.h"
#include "utils.h"
#include "SoftwareI2C.h"
#include <Seeed_Arduino_ooFreeRTOS.h>

#define SGP30_SDAPIN D1
#define SGP30_SCLPIN D0

using namespace cpp_freertos;

class Sgp30 : public Thread {

  public:
    Sgp30();
    int     data[2];
    uint8_t dsize;
    bool    status;
    SGP30       mySensor;
    SoftwareI2C softwarei2c;
  protected:
    virtual void Run();
};

class grove_sgp30_sensor : public sensor_base {
  public:
    grove_sgp30_sensor();
    void        init();
    const char *get_name();
    bool        read(struct sensor_data *data);

  private:
    const char *name = "CO2&VOC"; 
    int         sgp30_value[2];

    Sgp30 *sgp30;
};

#endif