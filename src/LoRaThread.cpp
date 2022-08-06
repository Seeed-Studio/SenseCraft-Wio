
#include "LoRaThread.h"

LoRaThread::LoRaThread(SysConfig &config, Message &m)
    : Thread("LoRaThread", 256, 1),
	  cfg(config),
      loraMail(m)
{
    Start();
};

void LoRaThread::Run()
{
	uint8_t nums;
    while (true)
    {

        // nums = loraMail.Receive(&sdata, 256, 0);
        // if (nums > 0)
        // {
        //     Serial.println("lora Receive");

        // }
		Serial.printf("lora thread %d\r\n", cfg.frequency);
	
		Delay(Ticks::MsToTicks(100));
    }
}

