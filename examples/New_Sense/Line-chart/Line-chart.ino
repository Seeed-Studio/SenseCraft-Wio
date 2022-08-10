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

//320*70 = 22400
void Sense_Display(int CHOOSE_PAGE) // Sense interface display
{
    spr.createSprite(320, 70);
    spr.setFreeFont(FSSB9);
    switch (CHOOSE_PAGE)
    {
    case 0:
        spr.fillRect(4 * PIXEL, 0, 21 * PIXEL, FONT_ROW_HEIGHT + 15, tft.color565(135, 206, 235));
        spr.fillRect(30 * PIXEL, 0, 21 * PIXEL, FONT_ROW_HEIGHT + 15, TFT_WHITE);
        spr.fillRect(56 * PIXEL, 0, 21 * PIXEL, FONT_ROW_HEIGHT + 15, TFT_WHITE);
        break;
    case 1:
        spr.fillRect(4 * PIXEL, 0, 21 * PIXEL, FONT_ROW_HEIGHT + 15, TFT_WHITE);
        spr.fillRect(30 * PIXEL, 0, 21 * PIXEL, FONT_ROW_HEIGHT + 15, tft.color565(135, 206, 235));
        spr.fillRect(56 * PIXEL, 0, 21 * PIXEL, FONT_ROW_HEIGHT + 15, TFT_WHITE);
        break;
    case 2:
        spr.fillRect(4 * PIXEL, 0, 21 * PIXEL, FONT_ROW_HEIGHT + 15, TFT_WHITE);
        spr.fillRect(30 * PIXEL, 0, 21 * PIXEL, FONT_ROW_HEIGHT + 15, TFT_WHITE);
        spr.fillRect(56 * PIXEL, 0, 21 * PIXEL, FONT_ROW_HEIGHT + 15, tft.color565(135, 206, 235));
        break;
    default:
        break;
    }

    spr.setFreeFont(FSSB9);
    spr.setTextColor(TFT_BLACK);
    spr.drawString("Sense", 32, 11, GFXFF);
    spr.drawString("Process", 127, 11, GFXFF);
    spr.drawString("Network", 231, 11, GFXFF);
    spr.drawLine(0, 2 * FONT_ROW_HEIGHT, SCREEN_WIDTH, 2 * FONT_ROW_HEIGHT, TFT_WHITE);

    spr.pushSprite(0, 0);
    spr.deleteSprite();
}


// 320 * 25 = 8000
void Network_state(int s_key)
{
    spr.createSprite(140, 25);
    spr.setFreeFont(FSSB9);
    spr.fillSprite(TFT_BLACK);

    switch (s_key)
    {
    case 0:
        spr.setTextColor(TFT_RED);
        spr.drawString("OFF", 60, 0, 2);
        break;
    case 1:
        spr.setTextColor(TFT_GREEN, TFT_BLACK);           // Networking status indication：ON
        spr.drawString("LoRa", 60, 0, 2); // Show the network you are in
        break;
    case 2:
        spr.setTextColor(TFT_GREEN, TFT_BLACK);          // Networking status indication：ON
        spr.drawString("WiFi", 60, 0, 2); // Show the network you are in
        break;
    default:;
    }
    spr.setTextColor(TFT_WHITE);
    spr.drawString("Network:", 5, 0, 2);
    spr.setFreeFont(FSS9);
    
    spr.pushSprite(0, 215);
    spr.deleteSprite();
}

void Below_Right_State_Content(int gg_state)  // SD 插拔状态 Grove 插拔状态
{
    
    spr.createSprite(320, 25);
    spr.setFreeFont(FSSB9);
    spr.fillSprite(TFT_BLACK);
    // int s_key = 1;
    switch (gg_state)
    {
    case 0:
        spr.setFreeFont(FSS9);
        spr.setTextColor(TFT_YELLOW);
        spr.drawString("Plug in a Grove sensor", 0, 0, 2);
        spr.pushSprite(170, 215);
        break;
    case 1:
        spr.setFreeFont(FSS9);
        spr.drawTriangle(0, 18, 11, 0, 22, 18, TFT_YELLOW);
        spr.setTextColor(TFT_YELLOW);
        spr.drawString("! ", 10, 4, 2);
        spr.drawString("TF card storage is full", 28, 0, 2);
        spr.pushSprite(140, 215);
        break;
    case 2:
        spr.setFreeFont(FSS9);
        spr.setTextColor(TFT_GREEN);
        spr.drawString("Vision AI Sensor connected", 22, 0, 2);
        spr.pushSprite(120, 215);
        break;
    case 3:
        spr.setFreeFont(FSS9);
        spr.setTextColor(TFT_GREEN);
        spr.drawString("Saving has been started", 22, 0, 2);
        spr.pushSprite(140, 215);
        break;
    case 4:
        spr.setFreeFont(FSS9);
        spr.setTextColor(TFT_YELLOW);
        spr.drawString("Please insert TF card", 22, 0, 2);
        spr.pushSprite(148, 215);
        break;
    
    default:;
    }
    spr.setTextColor(TFT_WHITE);
    spr.drawString("Network :", 5, 0, GFXFF);
    
    spr.deleteSprite();
}


void setup()
{
    pinMode(A0, INPUT);
    pinMode(WIO_LIGHT, INPUT);
    pinMode(WIO_KEY_A, INPUT_PULLUP);
    pinMode(WIO_KEY_B, INPUT_PULLUP);
    pinMode(WIO_KEY_C, INPUT_PULLUP);
    tft.begin();
    tft.setRotation(3);
    tft.fillScreen(TFT_BLACK);

}

int brightness;
int light;
int key_flag = 0;
int value_temp;
int test_flag = 0;

void loop()
{
    Sense_Display(0);
    if (digitalRead(WIO_KEY_B) == LOW)
    {

        Serial.println("B Key pressed");
        key_flag++;
        value_temp = 0;
        delay(200);
    }

    brightness = analogRead(WIO_MIC);
    light = analogRead(WIO_LIGHT);

    if (data.size() > DATA_MAX_SIZE) // keep the old line chart front
    {
        data.pop(); // this is used to remove the first read variable
    }

    value_temp = key_flag % 2;

    switch (value_temp)
    {
    case 0:
        data.push(brightness);
        Sense_Display(0);
        break;
    case 1:
        data.push(light);
        Sense_Display(1);//
        break;
    default:
        break;
    }

    // 85 * 260 = 22100
    auto content = line_chart(20, 80); //(x,y) where the line graph begins
    content
        .height(85)
        .width(260)
        .based_on(0.0)      // Starting point of y-axis, must be a float
        .show_circle(false) // drawing a cirle at each point, default is on.
        .value(data)        // passing through the data to line graph
        .max_size(MAX_SIZE)
        .color(TFT_GREEN) // Setting the color for the line
                          //        .backgroud(tft.color565(0,0,0)) // Setting the color for the backgroud
        .backgroud(tft.color565(0, 0, 0))
        .draw(&tft);

    if (digitalRead(WIO_KEY_B) == LOW){
       test_flag++;
       delay(300);
    }

    Network_state(1);
    Below_Right_State_Content(1);
    delay(20);
}
