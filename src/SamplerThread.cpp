
#include "SamplerThread.h"
#include "sensors/buildin_light_sensor.h"
#include "sensors/buildin_mic.h"
#include <vector>

SamplerThread::SamplerThread(Message &m1) : 
	Thread("SamplerThread", 4096, 1),
	sensorMail(m1)
{ 
	//start thread when created
	Start(); 
	
}

void SamplerThread::Run() {
  std::vector<sensor_base *> sensors;
  sensors.push_back(new buildin_light_sensor());
  sensors.push_back(new buildin_mic());

  while (true) {
    for (auto sensor : sensors) {
      sensor->read(&sdata);
      Serial.printf("Sampling %s\n", sdata.name);
      for (size_t i = 0; i < sdata.size; i++) {
        Serial.printf("%02x ", ((uint8_t *)sdata.data)[i]);
      }
      // for (auto sensor : sensors) {
      //   Serial.printf("Sampling %s\n", sensor->get_name());
      // }
      // Serial.println(sensors.size());
	  sensorMail.Send((void *)&sdata, sizeof(sdata));

      Serial.println("SamplerThread");
      Delay(Ticks::MsToTicks(100));
    }
  }
}
