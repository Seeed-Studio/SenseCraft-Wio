#ifndef __SDDATA_H__
#define __SDDATA_H__
#include <Seeed_Arduino_ooFreeRTOS.h>
#include "DateTime.h"
#include "RTC_SAMD51.h"
#include "SD/Seeed_SD.h"
#include "utils.h"
#include <CSV_Parser.h>
#include <SPI.h>
#include <Seeed_FS.h>

using namespace cpp_freertos;

class SDdata {
  public:
    SDdata();
    ~SDdata();
    bool                       init();
    uint8_t                    status();
    template <typename T> void saveData(String sensorName, T *sensorData, int len);
    void                       Readdata(String sensorName);

  private:
    RTC_SAMD51 rtc;
    File       myFile;
    bool       is_connected = false;
    	static SemaphoreHandle_t lock;
};

#endif // __SDDATA_H__