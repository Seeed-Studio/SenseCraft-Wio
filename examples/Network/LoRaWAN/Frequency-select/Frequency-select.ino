#include <TFT_eSPI.h>
#include "Free_Fonts.h"

TFT_eSPI tft;
TFT_eSprite spr = TFT_eSprite(&tft);

#define SCREEN_WIDTH 320                        // Wio Terminal Maximum Width
#define SCREEN_HIGH 240                         // Wio Terminal Maximum Height
#define PIXEL 4                                 // Width of one letter
const static unsigned int FONT_ROW_HEIGHT = 22; // The height of a letter


//Key
int key_status = 0;

void key()                 //设置按键
{
    if (digitalRead(WIO_KEY_C) == LOW) {
      Serial.println("A Key pressed");
      key_status = 1;
     }
     else if (digitalRead(WIO_KEY_B) == LOW) {
      Serial.println("B Key pressed");
      key_status = 2;
     }
     else if (digitalRead(WIO_KEY_A) == LOW) {
      Serial.println("C Key pressed");
      key_status = 3;
     }
}

void DISPLAY_INIT() // Display initialization, black background rotation
{
    tft.begin();
    tft.setRotation(3);
    tft.fillScreen(TFT_BLACK);
}

void Bandselect_Display(int key) // Select Frequency band interface
{
    spr.createSprite(SCREEN_WIDTH, SCREEN_HIGH);

    // put your main code here
    spr.setFreeFont(FSSB9);
    spr.setTextColor(TFT_BLACK);

    spr.fillRect(4 * PIXEL, 0, 21 * PIXEL, FONT_ROW_HEIGHT + 15, TFT_WHITE);
    spr.fillRect(30 * PIXEL, 0, 21 * PIXEL, FONT_ROW_HEIGHT + 15, TFT_WHITE);
    spr.fillRect(56 * PIXEL, 0, 21 * PIXEL, FONT_ROW_HEIGHT + 15, tft.color565(135, 206, 235));

    spr.fillRect(156, 54, 40, 4, tft.color565(220, 220, 220));

    spr.fillCircle(160, 55, 10, TFT_BLUE);                                    //button to the left to select LoRa network

    spr.drawString("Sense", 32, 11, GFXFF);
    spr.drawString("Process", 127, 11, GFXFF);
    
    spr.setTextColor(TFT_BLACK);
    spr.drawString("Network", 231, 11, GFXFF);
  
    spr.drawLine(0, 3.5 * FONT_ROW_HEIGHT, SCREEN_WIDTH, 3.5 * FONT_ROW_HEIGHT, TFT_WHITE);
  
    spr.setFreeFont(FSS9);
//    spr.setTextColor(TFT_WHITE, tft.color565(143, 195, 31));
    spr.setTextColor(TFT_WHITE, tft.color565(0, 139, 0));
    spr.drawString("    LoRa    ", 5, 48, GFXFF);  //LoRa(SenseCAP)
    
    spr.setTextColor(TFT_WHITE, tft.color565(100, 100, 100));
    spr.drawString("    WiFi    ", 205, 48, GFXFF);  //WiFi(Ubidots)

    spr.setTextColor(TFT_WHITE);
    spr.drawString("Select and confirm LoRaWAN frequency band", 20, 3.8 * FONT_ROW_HEIGHT, 2);

    if(key == 1)  //Press the A button to select the US915 band
    {
      spr.fillRect(18, 4.8 * FONT_ROW_HEIGHT, 80, 3.2 * FONT_ROW_HEIGHT, tft.color565(0, 139, 0));
      spr.drawString("US915 is commonly used in North America.", 25, 8.5 * FONT_ROW_HEIGHT, 2);
    }
    if(key == 2)  //Press the B button to select the EU868 band
    {
      spr.fillRect(118, 4.8 * FONT_ROW_HEIGHT, 80, 3.2 * FONT_ROW_HEIGHT, tft.color565(0, 139, 0));
      spr.drawString("EU868 is commonly used in the European region.", 10, 8.5 * FONT_ROW_HEIGHT, 2);
    }
    if(key == 3)  //Press the C button to select the AU915 band
    {
      spr.fillRect(218, 4.8 * FONT_ROW_HEIGHT, 80, 3.2 * FONT_ROW_HEIGHT, tft.color565(0, 139, 0));
      spr.drawString("AU915 is commonly used in Australia region.", 18, 8.5 * FONT_ROW_HEIGHT, 2);
    }

    spr.drawString("US", 20, 5 * FONT_ROW_HEIGHT, GFXFF);
    spr.drawString("EU", 120, 5 * FONT_ROW_HEIGHT, GFXFF);
    spr.drawString("AU", 220, 5 * FONT_ROW_HEIGHT, GFXFF);

    spr.setFreeFont(FSS24);
    spr.drawString("915", 20, 6 * FONT_ROW_HEIGHT, GFXFF);
    spr.drawString("868", 120, 6 * FONT_ROW_HEIGHT, GFXFF);
    spr.drawString("915", 220, 6 * FONT_ROW_HEIGHT, GFXFF);

    spr.setFreeFont(FSS9);
    spr.drawString("Network :", 5, 218 , GFXFF);
    
    spr.setTextColor(TFT_RED);             //Networking status indication：OFF
    spr.drawString("OFF", 82, 218 , GFXFF);

//    spr.setTextColor(TFT_GREEN, TFT_BLACK);             //Networking status indication：ON
//    spr.drawString("ON", 82, 218 , GFXFF);

    spr.pushSprite(0, 0);
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
    key();  //ABC button to select network
    Bandselect_Display(key_status);
}
