#include <TFT_eSPI.h>
#include "Free_Fonts.h"

TFT_eSPI tft;
TFT_eSprite spr = TFT_eSprite(&tft);

#define SCREEN_WIDTH 320                        // Wio Terminal Maximum Width
#define SCREEN_HIGH 240                         // Wio Terminal Maximum Height
#define PIXEL 4                                 // Width of one letter
const static unsigned int FONT_ROW_HEIGHT = 22; // The height of a letter

// Key
int key_status = 0;

void key() //设置按键
{
    if (digitalRead(WIO_KEY_C) == LOW)
    {
        Serial.println("A Key pressed");
        key_status = 1;
    }
    else if (digitalRead(WIO_KEY_B) == LOW)
    {
        Serial.println("B Key pressed");
        key_status = 2;
    }
    else if (digitalRead(WIO_KEY_A) == LOW)
    {
        Serial.println("C Key pressed");
        key_status = 3;
    }
}

void DISPLAY_INIT() // Display initialization, black background rotation
{
    tft.begin();
    tft.setRotation(3);
    tft.fillScreen(TFT_WHITE);
}

void Bandselect_Display(int key) // Select Frequency band interface
{
    //first line of information
    spr.createSprite(290, 30);

    spr.setFreeFont(FSS9);
    spr.setTextColor(TFT_WHITE);
    spr.drawString("Select and confirm LoRaWAN frequency band", 5, 5, 2);

    spr.pushSprite(20, 60);
    spr.deleteSprite();

    //
    spr.createSprite(290, 130);

    spr.setFreeFont(FSS9);
    spr.setTextColor(TFT_WHITE);
    if (key == 1) // Press the A button to select the US915 band
    {
        spr.fillRect(18, 105, 80, 70, tft.color565(0, 139, 0));
        spr.drawString("The US915 band is commonly used in North America.", 25, 187, 2);
    }
    if (key == 2) // Press the B button to select the EU868 band
    {
        spr.fillRect(118, 105, 80, 70, tft.color565(0, 139, 0));
        spr.drawString("The EU868 band is commonly used in the European region.", 10, 187, 2);
    }
    if (key == 3) // Press the C button to select the AU915 band
    {
        spr.fillRect(218, 105, 80, 70, tft.color565(0, 139, 0));
        spr.drawString("The AU915 band is commonly used in Australia region.", 18, 187, 2);
    }
    
    spr.setFreeFont(FSS9);
    spr.drawString("US", 5, 30, GFXFF);
    spr.drawString("EU", 105, 30, GFXFF);
    spr.drawString("AU", 205, 30, GFXFF);

    spr.setFreeFont(FSS24);
    spr.drawString("915", 5, 50, GFXFF);
    spr.drawString("868", 105, 50, GFXFF);
    spr.drawString("915", 205, 50, GFXFF);

    spr.fillRect(18, 105, 80, 70, tft.color565(0, 139, 0));
    spr.setFreeFont(FSS9);
    spr.setTextColor(TFT_WHITE);
    spr.drawString("The US915 band is commonly used in North America.", 5, 95, 2);
    spr.drawString("America.", 5, 110, 2);

    spr.pushSprite(20, 80);
    spr.deleteSprite();
}

void setup()
{
    DISPLAY_INIT();
    pinMode(WIO_KEY_C, INPUT_PULLUP);
    pinMode(WIO_KEY_B, INPUT_PULLUP);
    pinMode(WIO_KEY_A, INPUT_PULLUP);
}

void loop()
{
    key(); // ABC button to select network
    Bandselect_Display(key_status);
}
