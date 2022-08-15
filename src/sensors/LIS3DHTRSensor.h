#ifndef __LIS3DHTRSENSOR_H__
#define __LIS3DHTRSENSOR_H__
#include "sensor.h"
#include "utils.h"
#include "Wire.h"
#include "LIS3DHTR.h"



class LIS3DHTRSensor : public sensor_base
{
public:
    LIS3DHTRSensor();
    void init();
    bool read(struct sensor_data *data);
    const char *get_name();
private:
    const char *name = "IMU"; //LIS3DHTR
    int data[3];

    bool status;

    LIS3DHTR<TwoWire> lis;
};

#endif // __LIS3DHTRSENSOR_H__