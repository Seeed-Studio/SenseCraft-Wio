#include <TFT_eSPI.h> 
#include "Free_Fonts.h"

TFT_eSPI tft;
TFT_eSprite spr = TFT_eSprite(&tft); 

#define SCREEN_WIDTH 320                        // Wio Terminal Maximum Width
#define SCREEN_HIGH 240                         // Wio Terminal Maximum Height
#define PIXEL 4                                 // Width of one letter
#define LEFT_SIDE 70
#define HIGHT_SIDE 47
const static unsigned int FONT_ROW_HEIGHT = 22; // The height of a letter


void DISPLAY_INIT()            // Display initialization, black background rotation
{
    tft.begin();
    tft.setRotation(3);
    tft.fillScreen(TFT_BLACK);
}

void Sense_AutoDetecte_Display()                      //Display screen for accessing sensors
{
  spr.createSprite(SCREEN_WIDTH, SCREEN_HIGH);

  // put your main code here
  spr.setFreeFont(FSSB9);
  spr.setTextColor(TFT_BLACK, TFT_WHITE);

  spr.fillRect(4 * PIXEL, 0, 21 * PIXEL, FONT_ROW_HEIGHT + 15, tft.color565(135, 206, 235));
  spr.fillRect(30 * PIXEL, 0, 21 * PIXEL, FONT_ROW_HEIGHT + 15, TFT_WHITE);
  spr.fillRect(56 * PIXEL, 0, 21 * PIXEL, FONT_ROW_HEIGHT + 15, TFT_WHITE);

  spr.drawString("Process", 127, 11, GFXFF);
  spr.drawString("Network", 231, 11, GFXFF);

  spr.setTextColor(TFT_BLACK, tft.color565(135, 206, 235));
  spr.drawString("Sense", 32, 11, GFXFF);

  spr.drawLine(0, 2 * FONT_ROW_HEIGHT, SCREEN_WIDTH, 2 * FONT_ROW_HEIGHT, TFT_WHITE);

  spr.setFreeFont(FSS9);
  spr.setTextColor(TFT_BLACK, tft.color565(220, 220, 220));
  spr.fillRect(128, 50, 16 * PIXEL, FONT_ROW_HEIGHT, tft.color565(220, 220, 220));
  spr.drawString("Sensor", 132, 54, GFXFF);

  spr.setTextColor(TFT_WHITE, TFT_BLACK);
  spr.drawString("Sound", 100 - LEFT_SIDE, 80, GFXFF);
  spr.drawString("IMU", 209 - LEFT_SIDE, 80, GFXFF);
  spr.drawString("ADD", 316 - LEFT_SIDE, 80, GFXFF);

  spr.setFreeFont(FSS24);
  spr.drawString("356", 20, 115 , GFXFF);                      // Display the value of loudness
  spr.setFreeFont(FSS12);
  spr.drawString("0.10", 205 - LEFT_SIDE, 102 , GFXFF);        //Display the value of IMU X-axis
  spr.drawString("-0.23", 205 - LEFT_SIDE, 126 , GFXFF);       //Display the value of IMU Y-axis
  spr.drawString("-2.33", 205 - LEFT_SIDE, 150 , GFXFF);       //Display the value of IMU Z-axis
  spr.setFreeFont(FSS9);
  spr.drawString("X,Y,Z", 205 - LEFT_SIDE, 174 , GFXFF);

  spr.fillCircle(163, 200, 3, tft.color565(0, 193, 255));
  spr.fillCircle(146, 200, 3, tft.color565(220, 220, 220));

  spr.fillRect(244, 117, 40, 40, TFT_WHITE);
  spr.fillRect(263, 123, 3, 26, TFT_BLACK);
  spr.fillRect(251, 135, 26, 3, TFT_BLACK);
  
  spr.drawString("Network :", 5, 218 , GFXFF);

//  spr.setTextColor(TFT_RED, TFT_BLACK);                      //Networking status indication：OFF
//  spr.drawString("OFF", 82, 218 , GFXFF);

  spr.setTextColor(TFT_GREEN, TFT_BLACK);             //Networking status indication：ON
  spr.drawString("    LoRa    ", 82, 218 , GFXFF);  //Show the network you are in

  spr.pushSprite(0, 0);
  spr.deleteSprite();
}

void Insert_Tip()       // Pop-up prompt for sensor insertion
{
  spr.createSprite(240, 116);
  spr.fillScreen(tft.color565(211, 211, 211));

  spr.setFreeFont(FSS9);
  spr.setTextColor(TFT_BLACK, tft.color565(211, 211, 211));
  spr.drawString("Please plug in 1 sensor first", 10, 30, GFXFF);

  spr.fillCircle(108, 90, 3, tft.color565(190, 190, 190));
  spr.fillCircle(118, 90, 3, tft.color565(140, 140, 140));
  spr.fillCircle(128, 90, 3, tft.color565(105, 105, 105));

  spr.pushSprite(40, 64);
  spr.deleteSprite();
}

void Connect_Success_Display()     //Connect sensor success alert pop-up.
{
  spr.createSprite(240, 116);
  spr.fillScreen(tft.color565(211, 211, 211));

  spr.setFreeFont(FSS9);
  spr.setTextColor(TFT_BLACK, tft.color565(211, 211, 211));
  spr.drawString("Vision AI Sensor connected", 10, 30, GFXFF);

  spr.fillCircle(118, 85, 20, tft.color565(0, 139, 0));
  spr.fillCircle(118, 85, 17, tft.color565(211, 211, 211));

  //spr.drawLine(117, 86, 118, 89, tft.color565(0, 255, 0));
  spr.fillTriangle(107, 84, 128, 75, 118, 95, tft.color565(0, 139, 0));
  spr.fillTriangle(107, 80, 128, 71, 118, 91, tft.color565(211, 211, 211));

  spr.pushSprite(40, 64);
  spr.deleteSprite();
}

void setup() {
  DISPLAY_INIT();
}

void loop() {
  Sense_AutoDetecte_Display();
  delay(2000);
  Insert_Tip();
  delay(2000);
  Connect_Success_Display();
  delay(2000);
}
