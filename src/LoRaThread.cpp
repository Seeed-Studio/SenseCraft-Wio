
#include "LoRaThread.h"

LoRaThread::LoRaThread(SysConfig &config) : Thread("LoRaThread", 256, 1), cfg(config) {
    Start();
};

void LoRaThread::Run() {
    // init the library, search the LORAE5 over the different WIO port available
    lorae5 = new Disk91_LoRaE5(false);
    if (!lorae5->begin(DSKLORAE5_SWSERIAL_WIO_P2)) {
        Serial.println("LoRa E5 Init Failed");
        is_e5_connected = false;
    }

    while (true) {
        if (!lorae5->begin(DSKLORAE5_SWSERIAL_WIO_P2)) {
            is_e5_connected = false;
            Serial.println("LoRa E5 Init Failed");
        } else {
            is_e5_connected = true;
            Serial.println("LoRa E5 Init OK");
        }

        //print all data in the lora_data queue and pop it
        while (!lora_data.empty()) {
            Serial.println("LoRa E5 Sending Data");
            sensor_data data = lora_data.front();
            lora_data.pop();
            for (size_t i = 0; i < data.size; i = i + 4) {
                Serial.printf("%d ", ((int32_t *)data.data)[i]);
            }
            Serial.printf("sensor Receive: %s %d %d %d\r\n", data.name, data.id, data.size, lora_data.size());
        }
        Serial.printf("lora thread %s\r\n", cfg.lora_frequency.c_str());
        Delay(Ticks::MsToTicks(1000));
    }
}

//Store the received sensor data into a queue of length 30.
void LoRaThread::LoRaPushData(std::vector<sensor_data *> d) {
    // A loop to deep copy param of d vector into new lora_data queue 
    // by Iterative method
    for (auto data : d) {
        sensor_data *new_data = new sensor_data(*data);
        lora_data.push(*new_data);
    }
}