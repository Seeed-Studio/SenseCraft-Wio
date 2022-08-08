
#ifndef __SYSCONFIG_H__
#define __SYSCONFIG_H__
#include <Seeed_Arduino_ooFreeRTOS.h>
#include <Adafruit_TinyUSB.h>
#include <Seeed_Arduino_FS.h>
#include <Seeed_SFUD.h>
#include <sfud.h>


#include <stdint.h>
using namespace cpp_freertos;
class SysConfig
{
public:
	/* lora */
	uint8_t frequency;

public:
	SysConfig(/* args */);
	~SysConfig();
	void init();
	// void set_lora_freq(uint8_t frequency);
	// uint8_t get_lora_freq();
	static SemaphoreHandle_t lock;
private:
	/* data */
	bool sd_mount;
    bool spi_flash_mount;
	Adafruit_USBD_MSC usb_msc;
};

#endif // __SYSCONFIG_H__