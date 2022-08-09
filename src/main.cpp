
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



void display_init()            // Display initialization, black background rotation
{
    tft.begin();
    tft.setRotation(3);
    tft.fillScreen(TFT_BLACK);
}


void setup()
{

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
    Message *sensorMail = new Message(256);


    ButtonThread *btn = new ButtonThread(*btnMail);
    UI *u = new UI(tft,spr, *cfg, *btnMail, *sensorMail);
    SamplerThread *sampler = new SamplerThread(*cfg,*sensorMail);
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
