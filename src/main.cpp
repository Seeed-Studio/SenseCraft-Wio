
// /Users/baozhu/Library/Arduino15/packages/Seeeduino/tools/bossac/1.8.0-48-gb176eee/bossac -i -d --port=cu.usbmodem101 -U -i --offset=0x4000 -w -v .pio/build/seeed_wio_terminal/firmware.bin -R 

#include "Seeed_Arduino_ooFreeRTOS.h"
#include <Arduino.h>
#include <SPI.h>
#include "ui.h"
#include "ButtonThread.h"
#include "SamplerThread.h"
#include "LoRaThread.h"
#include "SysConfig.h"
#include "FreeRTOS.h"
#ifdef CM_DEBUG
#include "cm_backtrace.h"
#endif
#define HARDWARE_VERSION "seeed_k1100_dev_kit"
#define SOFTWARE_VERSION "V1.0.0"

using namespace cpp_freertos;

TFT_eSPI tft;
TFT_eSprite spr = TFT_eSprite(&tft); 


struct sensor_data1 {
     uint8_t   data[10];
    unsigned char size;
    uint8_t       id;
    bool          status; // 0: normal, 1: error
};


class testuiThread : public Thread {
  
public:
  
  testuiThread(int i, int delayInSeconds,TFT_eSPI &lcd, TFT_eSprite &display)
    : Thread( 1024, 1), 
      Id (i), 
      DelayInSeconds(delayInSeconds),
      tft(lcd), spr(display)
  {
    Start();
  };
  sensor_data sdata ;
  std::vector<sensor_data *> datas;
protected:

  virtual void Run() {
    while (true) {
      sdata.data = testdata;

        tft.fillRect(18, 78, 24, 90, TFT_WHITE);

        if (line_chart_data.size() > LINE_DATA_MAX_SIZE) // keep the old line chart front
        {
            line_chart_data.pop(); // this is used to remove the first read variable
        }

        line_chart_data.push(random(0, 10000)); // this is used to add a new variable to the end of the queue

        // 85 * 260 = 22100
        auto content = line_chart(20, 80); //(x,y) where the line graph begins
        content.height(85)
            .width(260)
            .based_on(0.0)          // Starting point of y-axis, must be a float
            .show_circle(false)     // drawing a cirle at each point, default is on.
            .value(line_chart_data) // passing through the data to line graph
            .max_size(LINE_DATA_MAX_SIZE)
            .color(TFT_GREEN) // Setting the color for the line
                            //        .backgroud(tft.color565(0,0,0)) // Setting the color for the
                            //        backgroud
            .backgroud(tft.color565(0, 0, 0))
            .draw(&tft);

    }
  };

private:
    TFT_eSPI    &tft;
    TFT_eSprite &spr;
    doubles line_chart_data;

  int Id;
  int DelayInSeconds;
  //10 random data
  uint8_t testdata[10] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A};
};

class testThread : public Thread {
  
public:
  
  testThread(int i, int delayInSeconds)
    : Thread( 256, 1), 
      Id (i), 
      DelayInSeconds(delayInSeconds)
  {
    Start();
  };
  sensor_data sdata ;
  std::vector<sensor_data *> datas;
protected:

  virtual void Run() {
    while (true) {
      sdata.data = testdata;
    //   memcpy(data->data, testdata, 10);
    //  data->name = "hello";
    //   datas.push_back(data);
     // LOGSS.printf("Blink Therad: %d\n\r", Id);
      digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
      Delay(Ticks::SecondsToTicks(DelayInSeconds));
      digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
      Delay(Ticks::SecondsToTicks(DelayInSeconds)); 
    //   datas.push_back(new sensor_data(*data));
    //   for(auto d : datas) {
    //     LOGSS.printf("data name: %s\n\r", d->name);
    //     delete d;
    //   }     

    //   datas.clear();
    //    datas.shrink_to_fit();
    }
  };

private:
  int Id;
  int DelayInSeconds;
  //10 random data
  uint8_t testdata[10] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A};
};

#if 1
class test1Thread : public Thread {
  
public:
  
  test1Thread(int i, int delayInms)
    : Thread( 256, 1), 
      Id (i), 
      DelayInSeconds(delayInms)
  {
    Start();
  };
  sensor_data sdata ;
protected:

  virtual void Run() {
    while (true) {
      std::vector<sensor_data *> datas;
      sdata.data = testdata;
      sdata.name = "hello";
    //   datas.push_back(data);

      Delay(Ticks::MsToTicks(DelayInSeconds)); 
      datas.push_back(new sensor_data(sdata));
      for(auto d : datas) {
        LOGSS.printf("data name: %s\n\r", d->name);
        delete d;
      }     
      datas.clear();
      datas.shrink_to_fit();
    }
  };

private:
  int Id;
  int DelayInSeconds;
  //10 random data
  uint8_t testdata[10] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A};
};

#endif


void display_init()            // Display initialization, black background rotation
{
    tft.begin();
    tft.setRotation(3);
    tft.fillScreen(TFT_BLACK);
}


void setup()
{
    LOGSS.begin(115200);

    #ifdef CM_DEBUG
    cm_backtrace_init("Seeed K1100 dev kit", HARDWARE_VERSION, SOFTWARE_VERSION);
    #endif
    pinMode(LED_BUILTIN, OUTPUT);
    testThread *test = new testThread(1, 1);
    test1Thread *test1 = new test1Thread(2, 3);
    test1Thread *test2 = new test1Thread(3, 1);
    test1Thread *test3 = new test1Thread(4, 1);
    test1Thread *test4 = new test1Thread(5, 3);
    display_init();
    testuiThread *testui = new testuiThread(1, 1, tft, spr);
    
#if 0
    SysConfig *cfg = new SysConfig(); 
    cfg->init();
    #ifdef CM_DEBUG
    cm_backtrace_init("Seeed K1100 dev kit", HARDWARE_VERSION, SOFTWARE_VERSION);
    #endif
    // put your setup code here, to run once:
    LOGSS.begin(115200);
    uint32_t start = millis();
    while (!LOGSS && (millis() - start) < 1500)
        ; // Open the Serial Monitor to get started or wait for 1.5"

    display_init();

    Message *btnMail = new Message(256);
    // Message *sensorMail = new Message(256);


    ButtonThread *btn = new ButtonThread(*btnMail);
    UI *u = new UI(tft,spr, *cfg, *btnMail);
    SamplerThread *sampler = new SamplerThread(*cfg, *u);
    #endif
}

//Get the size of memory left in the system in freertos.
int freeMemory() {
  return xPortGetFreeHeapSize();
}


void loop()
{
    LOGSS.printf("Main Stacks Free Bytes Remaining %d\r\n", uxTaskGetStackHighWaterMark(NULL));
    delay(1000);
}
