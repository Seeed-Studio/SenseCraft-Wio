#include "buildin_mic.h"
#include "Arduino.h"



buildin_mic::buildin_mic()
{
}
void buildin_mic::init()
{
	pinMode(WIO_MIC, INPUT);
}
bool buildin_mic::read(struct sensor_data *sdata)
{
	mic_value = analogRead(WIO_MIC);
	sdata->data = &mic_value;
	sdata->size = sizeof(mic_value);
	sdata->id = BUILDIN_MIC;
	sdata->name = name;
	sdata->status = true;
	return true;
}

const char *buildin_mic::get_name()
{
	return "buildin-mic";
}

