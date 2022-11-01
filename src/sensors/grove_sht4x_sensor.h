#ifndef __GROVE_SHT4X_SENSOR_H
#define __GROVE_SHT4X_SENSOR_H
#include "SHT4x.h"
#include "sensor.h"
#include "utils.h"
#include "SoftwareI2C.h"

#define SHT4X_SDAPIN D1
#define SHT4X_SCLPIN D0

class grove_sht4x_sensor : public sensor_base
{
public:
	grove_sht4x_sensor();
	const char *get_name();
	void init();
	bool read(struct sensor_data *data);
	bool is_connected;

private:
	const char *name = "T_H";
	const char *data_unit = "C,%RH";
	int         sht4x_value[2];
	SHT4x       sht4x;
};

#endif