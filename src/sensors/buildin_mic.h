#ifndef __BUILDIN_MIC_H
#define __BUILDIN_MIC_H

#include "sensor.h"
#include "utils.h"


class buildin_mic : public sensor_base
{
public:
	buildin_mic();
	void init();
	bool read(struct sensor_data *data);
	const char *get_name();
private:
	const char *name = "Sound"; //buildin-mic
	const char *data_unit = "dB";
	int mic_value;

};

// buildin_mic  buildin_mic;
#endif 