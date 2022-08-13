#ifndef __SHT40_H__
#define __SHT40_H__
#include "sensor.h"
#include "utils.h"
#include "Wire.h"
#include <SensirionI2CSht4x.h>

#define SHT4X_I2C_ADDRESS 0x44

class SHT40Sensor : public sensor_base
{
public:
    SHT40Sensor();
    void init();
    bool read(struct sensor_data *data);
    const char *get_name();
private:
    const char *name = "SHT40";
    int data[2];

    bool status;

    SensirionI2CSht4x sht4x;
};

#endif // __SHT40_H__