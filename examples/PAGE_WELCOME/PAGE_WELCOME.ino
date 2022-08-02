#include <TFT_eSPI.h>
#include <Wire.h>
#include <Seeed_Arduino_FreeRTOS.h>
#include "disk91_LoRaE5.h"
#include "LIS3DHTR.h"
#include "Seeed_Arduino_GroveAI.h"
#include "seeed_line_chart.h" 
#include <math.h>

TFT_eSPI tft = TFT_eSPI();
#define DATA_MAX_SIZE 30 // maximum size of data
#define MAX_SIZE 30 // maximum size of data
doubles data;  // Initilising a doubles type to store data
TFT_eSprite spr = TFT_eSprite(&tft); // Sprite
LIS3DHTR<TwoWire> lis;


// 显示尺寸设置
#define SCREEN_WIDTH 320
#define SCREEN_HIGH 240
#define SCREEN_PADDING 10
#define PIXEL 4
const static unsigned int FONT_SIZE = 2;        // rate
const static unsigned int FONT_ROW_HEIGHT = 22; // px
const static unsigned int FONT_LEFT_START = 0;  // px
const static unsigned int FONT_COLOR = TFT_WHITE;
const static unsigned int FONT_BG_COLOR = TFT_RED;
const static unsigned int BACKGROUD = TFT_BLACK;


void setup()
{
    pinMode(A0, INPUT);
    tft.begin();
    tft.setRotation(3);
    tft.fillScreen(TFT_WHITE);
}

void PAGE()  //设置频段页面
{
  tft.fillScreen(TFT_BLACK);
  tft.fillRect(0, 140, 320, 100, TFT_WHITE);
  tft.setTextSize(1);
  tft.setTextColor(TFT_BLACK);
  tft.setTextDatum(MC_DATUM);
  tft.fillRect(0, 0, 30, 10, TFT_CYAN);
  tft.fillRect(73, 0, 40, 10, TFT_GREEN);
  tft.fillRect(154, 0, 40, 10, TFT_YELLOW);
  tft.drawString("Please select LoRaWAN", 160, 160, 4);
  tft.drawString("frequency bands for your", 160, 190, 4);
  tft.drawString("device connection.", 160, 220, 4);

  tft.drawLine(12, 30, 12, 125, TFT_CYAN);
  tft.drawLine(12, 125, 245, 125, TFT_CYAN);

  tft.drawLine(90, 30, 90, 105, TFT_GREEN);
  tft.drawLine(90, 105, 245, 105, TFT_GREEN);

  tft.drawLine(168, 30, 168, 85, TFT_YELLOW);
  tft.drawLine(168, 85, 245, 85, TFT_YELLOW);

  tft.setTextColor(TFT_CYAN, TFT_BLACK);
  tft.drawString("Button A", 0, 23, 2);
  tft.drawString("EU868", 280, 125, 2);

  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.drawString("Button B", 95, 23, 2);
  tft.drawString("US915", 280, 105, 2);

  tft.setTextColor(TFT_YELLOW, TFT_BLACK);
  tft.drawString("Button C", 174, 23, 2);
  tft.drawString("AU915", 280, 85, 2);

  tft.setTextDatum(0);
}

void loop()
{
  PAGE();
}
