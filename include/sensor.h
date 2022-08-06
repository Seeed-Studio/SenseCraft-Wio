#ifndef __SENSOR_H
#define __SENSOR_H

typedef void (*sampler_callback)(const void *sample_buf, unsigned char byteLenght);


struct sensor_data
{
	const char *name;
	const void *data;
	unsigned char size;
	char id;
};

class sensor_base
{
public:
	virtual void init() = 0;
	virtual void read(struct sensor_data *data) = 0;
	virtual const char *get_name() = 0;
};

#endif