#ifndef __SOIL_H
#define __SOIL_H

#include "sensor.h"
#include "utils.h"


class SoilMoistureSensor : public sensor_base
{
public:
	SoilMoistureSensor();
	void init();
	bool read(struct sensor_data *data);
	const char *get_name();
private:
	const char *name = "soil-moisture";
	int soil_value;
	bool status;
    float calculateSD(int datas[], int len);
};

// __SOIL_H
#endif 