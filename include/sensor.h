#ifndef __SENSOR_H
#define __SENSOR_H

#include "SysConfig.h"
#include<vector>
#include "utils.h"

#define LOG_MAX_SIZE 8

typedef void (*sampler_callback)(const void *sample_buf, unsigned char byteLenght);


class sensor_base
{
public:
	static std::vector<log_data> slog;
	virtual void init() = 0;
	virtual bool read(struct sensor_data *data) = 0;
	virtual const char *get_name() = 0;
	void pushlog(const char *msg); 
};

#endif