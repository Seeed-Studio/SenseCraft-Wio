#ifndef __BUILDIN_LIGHT_SENSOR_H
#define __BUILDIN_LIGHT_SENSOR_H

#include "sensor.h"
#include "utils.h"

class buildin_light_sensor : public sensor_base
{
public:
	buildin_light_sensor();
	void init();
	const char *get_name();
	bool read(struct sensor_data *data);
private:
	const char *name = "Light"; /// buildin-light
	const char *data_unit = "Lux";
	int light_value;

};

#endif