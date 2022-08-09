
#include "Seeed_Arduino_ooFreeRTOS.h"
#include <Arduino.h>
#include <SPI.h>
#include "ui.h"
#include "ButtonThread.h"
#include "SamplerThread.h"
#include "LoRaThread.h"
#include "SysConfig.h"
#include "FreeRTOS.h"


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
    // put your setup code here, to run once:
    Serial.begin(9600);
    uint32_t start = millis();
    while (!Serial && (millis() - start) < 1500)
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
    Serial.print(F("\r\nFree RAM = ")); //F function does the same and is now a built in library, in IDE > 1.0.0
    Serial.println(freeMemory());  // print how much RAM is available in bytes.
    delay(1000);
}
