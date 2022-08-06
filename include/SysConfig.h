
#ifndef __SYSCONFIG_H__
#define __SYSCONFIG_H__

#include <stdint.h>
class SysConfig
{
public:
	/* lora */
	uint8_t frequency;

public:
	SysConfig(/* args */);
	~SysConfig();
	// void set_lora_freq(uint8_t frequency);
	// uint8_t get_lora_freq();
};

#endif // __SYSCONFIG_H__