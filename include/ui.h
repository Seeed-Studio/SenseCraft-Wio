#ifndef __UI_H__
#define __UI_H__
#include "Seeed_Arduino_ooFreeRTOS.h"
#include <Arduino.h>
#include "Free_Fonts.h"
#include <stdint.h>
#include <TFT_eSPI.h>


using namespace cpp_freertos;

#define SCREEN_WIDTH 320                        // Wio Terminal Maximum Width
#define SCREEN_HIGH 240                         // Wio Terminal Maximum Height


class UI: public Thread
{
public:
    UI(Message &m);
    void init();


protected:
    virtual void Run();

private:
    TFT_eSPI tft;
    TFT_eSprite spr = TFT_eSprite(&tft);

    Message &Mail;
    uint8_t buff[256];
};

#endif // __UI_H__
