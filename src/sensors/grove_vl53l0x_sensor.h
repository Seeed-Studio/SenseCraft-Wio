#ifndef __GROVE_VL53L0X_SENSOR_H
#define __GROVE_VL53L0X_SENSOR_H
#include "Seeed_vl53l0x.h"
#include "sensor.h"
#include "utils.h"
#include "SoftwareI2C.h"

#define VL53L0X_SDAPIN D1
#define VL53L0X_SCLPIN D0

class grove_vl53l0x_sensor : public sensor_base
{
public:
	grove_vl53l0x_sensor();
	const char *get_name();
	void init();
	bool read(struct sensor_data *data);
	bool is_connected;
private:
	const char *name = "Distance";
	const char *data_unit = "mm";
	int         vl53l0x_value;

	Seeed_vl53l0x VL53L0X;
};

#endif