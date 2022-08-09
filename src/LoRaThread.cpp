
#include "LoRaThread.h"

LoRaThread::LoRaThread(SysConfig &config) : Thread("LoRaThread", 256, 1), cfg(config) {
    Start();
};

void LoRaThread::Run() {
    // init the library, search the LORAE5 over the different WIO port available
    lorae5 = new Disk91_LoRaE5(false);
    if (!lorae5->begin(DSKLORAE5_SWSERIAL_WIO_P2)) {
        LOGSS.println("LoRa E5 Init Failed");
        is_e5_connected = false;
    }

    while (true) {
        if (!lorae5->begin(DSKLORAE5_SWSERIAL_WIO_P2)) {
            is_e5_connected = false;
            LOGSS.println("LoRa E5 Init Failed");
        } else {
            is_e5_connected = true;
            LOGSS.println("LoRa E5 Init OK");
        }

        //print all data in the lora_data queue and pop it
        while (!lora_data.empty()) {
            LOGSS.println("LoRa E5 Sending Data");
            sensor_data data = lora_data.front();
            lora_data.pop();
            lora_data_ready = true;
            for (size_t i = 0; i < data.size; i = i + 4) {
                LOGSS.printf("%d ", ((int32_t *)data.data)[i]);
            }
            LOGSS.printf("sensor Receive: %s %d %d %d\r\n", data.name, data.id, data.size, lora_data.size());
        }
        LOGSS.printf("lora thread %s\r\n", cfg.lora_frequency.c_str());
        LOGSS.printf("Lora Stacks Free Bytes Remaining %d\r\n", uxTaskGetStackHighWaterMark(GetHandle()));
        Delay(Ticks::MsToTicks(10));
    }
}

//Store the received sensor data into a queue of length 30.
void LoRaThread::LoRaPushData(std::vector<sensor_data *> d) {
    // A loop to deep copy param of d vector into new lora_data queue 
    // by Iterative method
    if(lora_data_ready)
        for (auto data : d) {
            // sensor_data *new_data = new sensor_data(*data);
            lora_data.push(*data);
    //        delete new_data;
            lora_data_ready = false;
        }
}