
#include "SamplerThread.h"
#include "sensors/FakeSensor.h"
#include "sensors/LIS3DHTRSensor.h"
#include "sensors/buildin_light_sensor.h"
#include "sensors/buildin_mic.h"
#include "sensors/buildex_sht4x_sensor.h"
#include "sensors/buildex_sgp30_sensor.h"
#include "sensors/buildex_soil_sensor.h"
#include <vector>

SamplerThread::SamplerThread(SysConfig &config, UI &ui)
    : Thread("SamplerThread", 256, 1), cfg(config), display(ui) {
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
    sensors.push_back(new buildex_sht4x_sensor());
	sensors.push_back(new buildex_sgp30_sensor());
    //sensors.push_back(new buildex_soil_sensor());
    // sensors.push_back(new FakeSensor());

    for (auto sensor : sensors) {
        sensor->init();
    }

    while (true) {
        std::vector<sensor_data *> datas;
        for (auto sensor : sensors) {
            if (sensor->read(&sdata)) {
                // LOGSS.printf("Sampling %s\r\n", sdata.name);
                //  for (size_t i = 0; i < sdata.size; i++) {
                //    LOGSS.printf("%02x ", ((uint8_t *)sdata.data)[i]);
                //  }
                //  for (auto sensor : sensors) {
                //    LOGSS.printf("Sampling %s\n", sensor->get_name());
                //  }
                //  LOGSS.println(sensors.size());
                // sensorMail.Send((void *)&sdata, sizeof(sdata));
                // deep Copy data into datas vector
                datas.push_back(new sensor_data(sdata));
            }
            // LOGSS.println("SamplerThread");
        }
        lora->LoRaPushData(datas);
        display.UIPushData(datas);
        for (auto data : datas) {
            delete data;
        }
        datas.clear();
        datas.shrink_to_fit();
        Delay(Ticks::MsToTicks(100));
        // LOGSS.printf("SamplerThread Stacks Free Bytes Remaining %d\r\n",
        // uxTaskGetStackHighWaterMark(GetHandle()));
    }
}
