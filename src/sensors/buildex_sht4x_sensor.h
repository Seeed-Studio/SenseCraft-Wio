#ifndef __BUILDEX_SHT4X_SENSOR_H
#define __BUILDEX_SHT4X_SENSOR_H
#include "SHT4x.h"
#include "sensor.h"
#include "utils.h"
#include "SoftwareI2C.h"

#define SHT4X_SDAPIN D1
#define SHT4X_SCLPIN D0

class buildex_sht4x_sensor : public sensor_base
{
public:
	buildex_sht4x_sensor();
	void init();
	const char *get_name();
	bool read(struct sensor_data *data);
private:
	const char *name = "Sht4x sensor"; /// buildin-light
	int sht4x_value[2];

	SHT4x  sht4x;
	SoftwareI2C softwarei2c;
};

#endif