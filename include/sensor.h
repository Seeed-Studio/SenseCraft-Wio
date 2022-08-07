#ifndef __SENSOR_H
#define __SENSOR_H

typedef void (*sampler_callback)(const void *sample_buf, unsigned char byteLenght);


class sensor_base
{
public:
	virtual void init() = 0;
	virtual bool read(struct sensor_data *data) = 0;
	virtual const char *get_name() = 0;
};

#endif