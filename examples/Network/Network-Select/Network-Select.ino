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

void Netselect_Display(int key) // Select network interface
{
    spr.createSprite(SCREEN_WIDTH, SCREEN_HIGH);

    // put your main code here
    spr.setFreeFont(FSSB9);
    spr.setTextColor(TFT_BLACK, TFT_WHITE);

    spr.fillRect(4 * PIXEL, 0, 21 * PIXEL, FONT_ROW_HEIGHT + 15, TFT_WHITE);
    spr.fillRect(30 * PIXEL, 0, 21 * PIXEL, FONT_ROW_HEIGHT + 15, TFT_WHITE);
    spr.fillRect(56 * PIXEL, 0, 21 * PIXEL, FONT_ROW_HEIGHT + 15, tft.color565(135, 206, 235));

//    spr.fillRect(30 * PIXEL, 0, 21 * PIXEL, FONT_ROW_HEIGHT + 15, tft.color565(100, 100, 100));
//    spr.fillRect(30 * PIXEL, 0, 21 * PIXEL, FONT_ROW_HEIGHT + 15, tft.color565(100, 100, 100));

    spr.fillRect(156, 54, 40, 4, tft.color565(220, 220, 220));

    if(key == 0)spr.fillCircle(175, 56, 10, tft.color565(190, 190, 190));                 //Default in no network state selected
    if(key == 1)spr.fillCircle(160, 55, 10, TFT_BLUE);                                    //button to the left to select LoRa network
    if(key == 2)spr.fillCircle(175, 56, 10, tft.color565(190, 190, 190));                 //button to the middle, do not select the network
    if(key == 3)spr.fillCircle(192, 55, 10, TFT_BLUE);                                    //button to the right to select the WiFi network

    spr.drawString("Sense", 32, 11, GFXFF);
    spr.drawString("Process", 127, 11, GFXFF);
    
    spr.setTextColor(TFT_BLACK, tft.color565(135, 206, 235));
    spr.drawString("Network", 231, 11, GFXFF);
  
    spr.drawLine(0, 3.5 * FONT_ROW_HEIGHT, SCREEN_WIDTH, 3.5 * FONT_ROW_HEIGHT, TFT_WHITE);
  
    spr.setFreeFont(FSS9);
    spr.setTextColor(TFT_WHITE, tft.color565(100, 100, 100));
    spr.drawString("LoRa(SenseCAP)", 5, 48, GFXFF);
    spr.drawString("WiFi(Ubidots)", 205, 48, GFXFF);

    spr.setTextColor(TFT_WHITE, TFT_BLACK);
    spr.drawString("Please press the confirmation", 30, 95, GFXFF);
    spr.drawString("button, then use left and right", 30, 95 + FONT_ROW_HEIGHT, GFXFF);
    spr.drawString("arrow keys to select your network", 30, 95 + 2 * FONT_ROW_HEIGHT, GFXFF);
    spr.drawString("connection method.", 30, 95 + 3 * FONT_ROW_HEIGHT, GFXFF);

      
    spr.drawString("Network :", 5, 218 , GFXFF);
    

//  spr.setTextColor(TFT_RED, TFT_BLACK);             //Networking status indication：OFF
//  spr.drawString("OFF", 82, 218 , GFXFF);

    spr.setTextColor(TFT_GREEN, TFT_BLACK);             //Networking status indication：ON
    spr.drawString("ON", 82, 218 , GFXFF);

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
    Netselect_Display(key_status);
}
