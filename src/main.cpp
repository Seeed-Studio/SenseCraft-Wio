
#include "ButtonThread.h"
#include "FreeRTOS.h"
#include "LoRaThread.h"
#include "SamplerThread.h"
#include "Seeed_Arduino_ooFreeRTOS.h"
#include "SysConfig.h"
#include "SDdata.h"
#include "ui.h"
#include <Arduino.h>
#include <SPI.h>
#ifdef CM_DEBUG
#include "cm_backtrace.h"
#endif
#define HARDWARE_VERSION "seeed_k1100_dev_kit"
#define SOFTWARE_VERSION "V1.0.0"

using namespace cpp_freertos;

TFT_eSPI    tft;
TFT_eSprite spr = TFT_eSprite(&tft);

void display_init() // Display initialization, black background rotation
{
    tft.begin();
    tft.setRotation(3);
    tft.fillScreen(TFT_BLACK);
    spr.createSprite(320, 120);
    spr.setTextColor(TFT_WHITE);
    spr.setFreeFont(FSSB9);
    // code to view the tutorial
    spr.drawString("SenseCAP K1100", 90, 0, GFXFF);
    spr.setFreeFont(FSS9);
    spr.drawString(VERSION, 150, 40, FONT2);
    spr.pushSprite(0, 110);
    spr.deleteSprite();
}

void setup() {

    SysConfig *cfg = new SysConfig();
    cfg->init();

    SDdata *sd = new SDdata();
    sd->init();
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

    ButtonThread  *btn     = new ButtonThread(*btnMail);
    UI            *u       = new UI(tft, spr, *cfg, *sd, *btnMail);
    SamplerThread *sampler = new SamplerThread(*cfg, *u);
}

// Get the size of memory left in the system in freertos.
int freeMemory() {
    return xPortGetFreeHeapSize();
}

void loop() {
    // LOGSS.printf("Main Stacks Free Bytes Remaining %d\r\n", uxTaskGetStackHighWaterMark(NULL));
    delay(100);
    vTaskDelete(NULL);
}
