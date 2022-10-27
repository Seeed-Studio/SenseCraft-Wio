#ifndef __GROVE_SHT4X_SENSOR_H
#define __GROVE_SHT4X_SENSOR_H
#include "SHT4x.h"
#include "sensor.h"
#include "utils.h"
#include "SoftwareI2C.h"
#include <Seeed_Arduino_ooFreeRTOS.h>

using namespace cpp_freertos;

#define SHT4X_SDAPIN D1
#define SHT4X_SCLPIN D0

class Sht4x : public Thread {

  public:
    Sht4x();
    int32_t     data[2];
    uint8_t dsize;
    bool    status;
	SHT4x  sht4x_dev;
	// SoftwareI2C softwarei2c;

  protected:
    virtual void Run();
};


class grove_sht4x_sensor : public sensor_base
{
public:
	grove_sht4x_sensor();
	void init();
	const char *get_name();
	bool read(struct sensor_data *data);
private:
	const char *name = "T_H";
	const char *data_unit = "C,%RH";
	int         sht4x_value[2];
	bool        is_connected;
	SHT4x       sht4x;
	// SoftwareI2C softwarei2c;
	// Sht4x  *sht4x;
};

#endif