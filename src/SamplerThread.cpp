
#include "SamplerThread.h"
#include "sensors/FakeSensor.h"
#include "sensors/LIS3DHTRSensor.h"
#include "sensors/buildin_light_sensor.h"
#include "sensors/buildin_mic.h"
#include <vector>

SamplerThread::SamplerThread(SysConfig &config, Message &m1)
    : Thread("SamplerThread", 256, 1), cfg(config), sensorMail(m1) {
    // start thread when created
    Start();
}

void SamplerThread::Run() {
    wifi = new WiFiThread(cfg);
    lora = new LoRaThread(cfg);

    std::vector<sensor_base *> sensors;
    sensors.push_back(new buildin_light_sensor());
    sensors.push_back(new buildin_mic());
    sensors.push_back(new LIS3DHTRSensor());
    sensors.push_back(new FakeSensor());

   

    for (auto sensor : sensors) {
        sensor->init();
    }

    while (true) {
        std::vector<sensor_data *> datas;
        for (auto sensor : sensors) {
            if (sensor->read(&sdata)) {
                LOGSS.printf("Sampling %s\n", sdata.name);
                // for (size_t i = 0; i < sdata.size; i++) {
                //   LOGSS.printf("%02x ", ((uint8_t *)sdata.data)[i]);
                // }
                // for (auto sensor : sensors) {
                //   LOGSS.printf("Sampling %s\n", sensor->get_name());
                // }
                // LOGSS.println(sensors.size());
                sensorMail.Send((void *)&sdata, sizeof(sdata));
                //deep Copy data into datas vector
                datas.push_back(new sensor_data(sdata));
            }
            // LOGSS.println("SamplerThread");
            Delay(Ticks::MsToTicks(100));
        }
        lora->LoRaPushData(datas);
        datas.clear();
        LOGSS.printf("SamplerThread Stacks Free Bytes Remaining %d\r\n", uxTaskGetStackHighWaterMark(GetHandle()));
    }
}
