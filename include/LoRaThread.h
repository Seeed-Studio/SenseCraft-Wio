
#ifndef __LORATHREAD_H__
#define __LORATHREAD_H__

#include "Arduino.h"
#include "Seeed_Arduino_ooFreeRTOS.h"
#include "utils.h"
#include "SysConfig.h"
#include "disk91_LoRaE5.h"
#include <queue>


// create a buttion class use ooFreeRTOS task

using namespace cpp_freertos;

class LoRaThread : public Thread
{
public:
    LoRaThread(SysConfig &config);
    void LoRaPushData(std::vector<sensor_data *> d);

protected:
    virtual void Run();

private:
	SysConfig &cfg;
    Disk91_LoRaE5 *lorae5;

    bool is_e5_connected;
    std::queue<sensor_data>  lora_data;
    bool lora_data_ready = true;

};

#endif // __LORATHREAD_H__