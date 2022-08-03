#include <TFT_eSPI.h>
#include <Wire.h>
#include <Seeed_Arduino_FreeRTOS.h>
#include "disk91_LoRaE5.h"
#include "LIS3DHTR.h"
#include "Seeed_Arduino_GroveAI.h"
#include "seeed_line_chart.h"
#include <math.h>
#include "Free_Fonts.h"

TFT_eSPI tft = TFT_eSPI();
#define DATA_MAX_SIZE 30             // maximum size of data
#define MAX_SIZE 30                  // maximum size of data
doubles data;                        // Initilising a doubles type to store data
TFT_eSprite spr = TFT_eSprite(&tft); // Sprite
LIS3DHTR<TwoWire> lis;
#define SCREEN_WIDTH 320 // Wio Terminal Maximum Width
#define SCREEN_HIGH 240  // Wio Terminal Maximum Height
#define PIXEL 4          // Width of one letter

// 显示尺寸设置
#define SCREEN_WIDTH 320
#define SCREEN_HIGH 240
#define SCREEN_PADDING 0
#define PIXEL 4
const static unsigned int FONT_SIZE = 2;        // rate
const static unsigned int FONT_ROW_HEIGHT = 22; // px
const static unsigned int FONT_LEFT_START = 0;  // px
const static unsigned int FONT_COLOR = TFT_WHITE;
const static unsigned int FONT_BG_COLOR = TFT_RED;
const static unsigned int BACKGROUD = TFT_BLACK;

void DISPLAY_INIT() // Display initialization, black background rotation
{
    tft.begin();
    tft.setRotation(3);
    tft.fillScreen(TFT_BLACK);
}

void Sense_Display() // Sense interface display
{
    spr.createSprite(SCREEN_WIDTH, SCREEN_HIGH);

    spr.setFreeFont(FSSB9);
    spr.setTextColor(TFT_BLACK, TFT_WHITE);

    spr.fillRect(4 * PIXEL, 0, 21 * PIXEL, FONT_ROW_HEIGHT + 15, TFT_WHITE);
    spr.fillRect(30 * PIXEL, 0, 21 * PIXEL, FONT_ROW_HEIGHT + 15, TFT_WHITE);
    spr.fillRect(56 * PIXEL, 0, 21 * PIXEL, FONT_ROW_HEIGHT + 15, TFT_WHITE);

    spr.drawString("Sense", 32, 11, GFXFF);
    spr.drawString("Process", 127, 11, GFXFF);
    spr.drawString("Network", 231, 11, GFXFF);

    spr.drawLine(0, 2 * FONT_ROW_HEIGHT, SCREEN_WIDTH, 2 * FONT_ROW_HEIGHT, TFT_WHITE);

    spr.setFreeFont(FSS9);
    spr.setTextColor(TFT_BLACK, tft.color565(220, 220, 220));
    spr.fillRect(127, 53.5, 16 * PIXEL, FONT_ROW_HEIGHT, tft.color565(220, 220, 220));
    spr.drawString("Sensor", 130, 56, GFXFF);
}

void setup()
{
    pinMode(A0, INPUT);
    tft.begin();
    tft.setRotation(3);
    tft.fillScreen(TFT_WHITE);
    //    DISPLAY_INIT();
}

int brightness;
void loop()
{
    //    brightness = analogRead(A0);

    Sense_Display();
    //    spr.fillRect(0, 80, 320, 150, TFT_WHITE);
    brightness = analogRead(WIO_MIC);

    if (data.size() > DATA_MAX_SIZE) // keep the old line chart front
    {
        data.pop(); // this is used to remove the first read variable
    }

    data.push(brightness); // read variables and store in data

    // Settings for the line graph
    auto content = line_chart(20, 80); //(x,y) where the line graph begins
    content
        //        .height(tft.height() - header.height() * 1.5 - 50) // actual height of the line chart
        //        .width(tft.width() - content.x() * 2)              // actual width of the line chart
        .height(120)
        .width(260)
        .based_on(0.0)      // Starting point of y-axis, must be a float
        .show_circle(false) // drawing a cirle at each point, default is on.
        .value(data)        // passing through the data to line graph
        .max_size(MAX_SIZE)
        .color(TFT_GREEN)     // Setting the color for the line
        .backgroud(TFT_WHITE) // Setting the color for the backgroud
        .draw(&tft);

    spr.fillRect(0, 201, 320, 39, TFT_BLACK);
    spr.setTextColor(TFT_YELLOW);
    spr.drawString("Network:", 10, 220, GFXFF);
    spr.setTextColor(tft.color565(254, 0, 0));
    spr.drawString("OFF", 100, 220, GFXFF);
    spr.pushSprite(0, 0);
    spr.deleteSprite();
    delay(2);
}
