
#ifndef __LORATHREAD_H__
#define __LORATHREAD_H__

#include "Arduino.h"
#include "Seeed_Arduino_ooFreeRTOS.h"
#include "utils.h"
#include "SysConfig.h"

// create a buttion class use ooFreeRTOS task

using namespace cpp_freertos;

class LoRaThread : public Thread
{
public:
    LoRaThread(SysConfig &config, Message &m);

protected:
    virtual void Run();

private:
    Message &loraMail;

	SysConfig &cfg;
};

#endif // __LORATHREAD_H__