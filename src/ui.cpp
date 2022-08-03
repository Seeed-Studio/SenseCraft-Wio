#include "ui.h"
#include <Arduino.h>
#include "Free_Fonts.h"
UI::UI()
{
}

UI::~UI()
{
}
void UI::init()
{
    tft.begin();
    tft.setRotation(3);
    tft.fillScreen(TFT_BLACK);
    
    spr.createSprite(SCREEN_WIDTH, SCREEN_HIGH);
}
