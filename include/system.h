#ifndef __XSYSTEM_H__
#define __XSYSTEM_H__

#include <Arduino.h>
#include <rpcWiFi.h>
#include <Adafruit_TinyUSB.h>
#include <Seeed_Arduino_ooFreeRTOS.h>
#include <Seeed_Arduino_FS.h>
#include <Seeed_SFUD.h>
#include <sfud.h>

using namespace cpp_freertos;

class System
{
public:
    System(/* args */);
    ~System();
    void begin();
    String SSID();
    String PWD();
    bool TF();
    uint32_t write(uint8_t *data, uint32_t size);
    uint32_t write(char *data)
    {
        return write((uint8_t *)data, strlen(data));
    };
    uint32_t write(String data)
    {
        return write((uint8_t *)data.c_str(), data.length());
    };
    static SemaphoreHandle_t lock;

private:
    bool sd_mount;
    bool flash_mount;
    Adafruit_USBD_MSC usb_msc;
};

extern System sys;

#endif /*__XSYSTEM_H__*/