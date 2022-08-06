#ifndef __BUILDIN_MIC_H
#define __BUILDIN_MIC_H

#include "sensor.h"

class buildin_mic : public sensor_base
{
public:
	buildin_mic();
	void init();
	void read(struct sensor_data *data);
	const char *get_name();
private:
	const char *name = "buildin-mic";
	int mic_value;

};

// buildin_mic  buildin_mic;
#endif 