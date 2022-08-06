#ifndef __XSYSTEM_H__
#define __XSYSTEM_H__

#include <Arduino.h>
#include <rpcWiFi.h>
#include <Adafruit_TinyUSB.h>
#include <Seeed_Arduino_FS.h>
#include <Seeed_SFUD.h>
#include <sfud.h>

class CC
{
public:
    CC(/* args */);
    ~CC();
    void begin();
    bool mount();
    void data_write(uint8_t *buff, uint32_t size_t);

private:
    bool sd_mount;
    bool flash_mount;
    Adafruit_USBD_MSC usb_msc;
};

extern CC sys;

#endif /*__XSYSTEM_H__*/