#ifndef __SGP30_H__
#define __SGP30_H__
#include "sensor.h"
#include "utils.h"
#include "Wire.h"
#include "sgp30.h"
#include "sensirion_common.h"

#define SGP_I2C_ADDRESS 0x58

class SGP30Sensor : public sensor_base
{
public:
    SGP30Sensor();
    void init();
    bool read(struct sensor_data *data);
    const char *get_name();
private:
    const char *name = "SGP30";
    int data[2];

    bool status;
};

#endif // __SGP30_H__