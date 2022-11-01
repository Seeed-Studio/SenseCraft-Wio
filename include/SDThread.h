#ifndef __SDTHREAD_H__
#define __SDTHREAD_H__
#include "DateTime.h"
#include "RTC_SAMD51.h"
#include "SD/Seeed_SD.h"
#include "SysConfig.h"
#include "utils.h"
#include <CSV_Parser.h>
#include <SPI.h>
#include <Seeed_Arduino_ooFreeRTOS.h>
#include <Seeed_FS.h>
#include <vector>

using namespace cpp_freertos;

class SDThread : public Thread {
  public:
    SDThread(SysConfig &config);
    void SDPushData(std::vector<sensor_data *> d);

  protected:
    virtual void Run();
    uint8_t      status();
    void         saveData(String sensorName, int32_t *sensorData, int len, uint8_t type);
    void         Readdata(String sensorName);

  private:
    SysConfig &cfg;

  private:
    std::vector<sensor_data> sd_data;
    bool                     sd_data_ready = true;
    RTC_SAMD51               rtc;
    File                     myFile;
    bool                     is_connected = false;
    SemaphoreHandle_t        wait_sd_data = NULL;
};

#endif // __SDTHREAD_H__