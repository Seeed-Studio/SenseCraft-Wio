
#ifndef __SYSCONFIG_H__
#define __SYSCONFIG_H__
#include <Seeed_Arduino_ooFreeRTOS.h>
#include <Adafruit_TinyUSB.h>
#include <Seeed_Arduino_FS.h>
#include <Seeed_SFUD.h>
#include <sfud.h>
#include "utils.h"


#include <stdint.h>
using namespace cpp_freertos;
class SysConfig
{
public:
	/* lora */
	String ssid;
	String password;
	String mqtt_client_name;
	String token;
	String device_label;
	uint8_t  lora_frequency;
	bool  lora_on = false;
	bool  wifi_on = false;
	bool  wificonnected = false;
	lora_status_type lora_status = LORA_INIT_START;
	int16_t lora_rssi = 0;
	int16_t lora_fcnt = 0;
	int16_t lora_sucess_cnt = 0;


	static SemaphoreHandle_t lock;

public:
	SysConfig(/* args */);
	~SysConfig();
	void init();
	void ReadAllConfig();
	void WriteConfigParam(char* filename , char *prefix_param, char *param);
private:
	/* data */
    bool spi_flash_mount;
	Adafruit_USBD_MSC usb_msc;
	

	void ReadConfigParam(const String filename,  char *prefix_param, String *param);
};

#endif // __SYSCONFIG_H__