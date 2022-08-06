
#include "Seeed_Arduino_ooFreeRTOS.h"
#include <Arduino.h>
#include <SPI.h>
#include "ui.h"
#include "ButtonThread.h"
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
    // put your setup code here, to run once:
    Serial.begin(9600);
    uint32_t start = millis();
    while (!Serial && (millis() - start) < 1500)
        ; // Open the Serial Monitor to get started or wait for 1.5"

    display_init();

    Message *Mail;
    Mail = new Message(256);
    ButtonThread *btn = new ButtonThread(*Mail);
    UI *u = new UI(tft,spr, *Mail);
    Thread::StartScheduler();
}

void loop()
{
}
