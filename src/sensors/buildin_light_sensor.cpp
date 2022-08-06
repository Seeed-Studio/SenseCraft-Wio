
#include "buildin_light_sensor.h"
#include "Arduino.h"



buildin_light_sensor::buildin_light_sensor()
{
}
void buildin_light_sensor::init()
{
	pinMode(WIO_LIGHT, INPUT);
}

void buildin_light_sensor::read(struct sensor_data *sdata)
{
	light_value = analogRead(WIO_LIGHT);
	sdata->data = &light_value;
	sdata->size = sizeof(light_value);
	sdata->id = 1;
	sdata->name = name;

}

const char *buildin_light_sensor::get_name()
{
	return "light";
}

// buildin_light_sensor  buildin_light;