#include "Seeed_Arduino_ooFreeRTOS.h"
#include <Arduino.h>
#include "ui.h"
#include "ButtonThread.h"
using namespace cpp_freertos;

void setup()
{
    // put your setup code here, to run once:
    Serial.begin(9600);
    uint32_t start = millis();
    while (!Serial && (millis() - start) < 1500)
        ; // Open the Serial Monitor to get started or wait for 1.5"
        
    Message *Mail;
    Mail = new Message(256);
    ButtonThread *btn = new ButtonThread(*Mail);
    UI *u = new UI(*Mail);
    Thread::StartScheduler();
}

void loop()
{
}
