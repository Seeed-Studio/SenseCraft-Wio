
#ifndef __LORATHREAD_H__
#define __LORATHREAD_H__

#include "Arduino.h"
#include "Seeed_Arduino_ooFreeRTOS.h"
#include "SysConfig.h"
#include "disk91_LoRaE5.h"
#include "utils.h"
#include <vector>

// create a buttion class use ooFreeRTOS task

using namespace cpp_freertos;

class LoRaThread : public Thread {
  public:
    LoRaThread(SysConfig &config);
    void LoRaPushData(std::vector<sensor_data *> d);

  private:
    void Init();
    void Join();
    bool SendDeviceInfo();
    bool SendBuildinSensorData();
    bool SendGroveSensorData();
    bool SendData(uint8_t *data, uint8_t len, uint8_t ver);

  protected:
    virtual void Run();

  private:
    SysConfig     &cfg;
    Disk91_LoRaE5 *lorae5;
    const uint8_t  v1 = 2;
    const uint8_t  v2 = 3;
    uint8_t        downlink_rxBuff[16];
    uint8_t        downlink_rxSize = 16;
    uint8_t        downlink_rxPort;
    uint8_t        frequency;

    std::vector<sensor_data> lora_data;
    bool                     lora_data_ready = true;
};

#endif // __LORATHREAD_H__