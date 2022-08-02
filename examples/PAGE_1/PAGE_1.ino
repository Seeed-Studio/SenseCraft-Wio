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

#define sFF17 "Sans 9"
#define sFF18 "Sans 12"
#define sFF19 "Sans 18"
#define sFF20 "Sans 24"

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
int conf = 0;                                     //置信度
int num = 0;                                      //人体数量
int light = 0, mic_val = 0;                       //光线值、麦克风响度
bool col = false;                                 //控制LoRaWAN灯色
float x_values = 0.0, y_value = 0.0, z_val = 0.0; // IMU数值
int conf_avg = 0;

void setup()
{
    pinMode(A0, INPUT);
    tft.begin();
    tft.setRotation(3);
    tft.fillScreen(TFT_BLACK);
    spr.setFreeFont(FF18);
}

void Page_1()
{

    char str_num[1], str_conf[1], str_light[1], str_mic_val[1], str_x_values[8], str_y_value[8], str_z_val[8];
    sprintf(str_num, "%d", num);
    sprintf(str_conf, "%d", conf_avg);
    sprintf(str_light, "%d", light);
    sprintf(str_mic_val, "%d", mic_val);
    snprintf((char *)str_x_values, 8, "%.2f", x_values);
    snprintf((char *)str_y_value, 8, "%.2f", y_value);
    snprintf((char *)str_z_val, 8, "%.2f", z_val);

    conf_avg = 0;

    spr.createSprite(320, 240);

    spr.fillRect(0, 32, 320, 28, tft.color565(61, 145, 64));
    spr.fillRect(0, 60, 320, 100, tft.color565(163, 148, 128));

    spr.setTextColor(TFT_WHITE); //设置字体颜色和背景颜色

    spr.fillRect(10, 0, 75, 30, TFT_WHITE);
    spr.fillRect(90, 0, 95, 30, TFT_WHITE);
    spr.fillRect(190, 0, 100, 30, TFT_WHITE);

    spr.setTextColor(TFT_BLACK);                                            //设置字体颜色和背景颜色
    spr.drawString("  Sense  Process  Network", FONT_LEFT_START, 5, FONT4); //写字（参数：字符串、开始x坐标、开始y坐标、字体）

    spr.drawLine(0, 35, 320, 35, TFT_WHITE);

    spr.setTextColor(TFT_WHITE);
    spr.drawString("  Sensor ", 90, 35, FONT4); //写字（参数：字符串、开始x坐标、开始y坐标、字体）

    spr.drawString("  Average", FONT_LEFT_START, 32 + 2.5 * FONT_ROW_HEIGHT, 2);
    spr.drawString("  Accuracy:", FONT_LEFT_START, 32 + 3.5 * FONT_ROW_HEIGHT, 2);
    spr.drawString("%", FONT_LEFT_START + 100, 32 + 3.5 * FONT_ROW_HEIGHT, 2);
    spr.drawString("Light: ", FONT_LEFT_START + 149, 32 + 1.5 * FONT_ROW_HEIGHT, 2);
    spr.drawString("Sound: ", FONT_LEFT_START + 149, 32 + 2.5 * FONT_ROW_HEIGHT, 2);
    spr.drawString("IMU: ", FONT_LEFT_START + 149, 32 + 3.5 * FONT_ROW_HEIGHT, 2);
    spr.drawString("(x,y,z)", FONT_LEFT_START + 149, 32 + 4.5 * FONT_ROW_HEIGHT, 2);

    spr.setTextColor(TFT_WHITE); //设置字体颜色和背景颜色

    spr.setTextColor(tft.color565(25, 25, 112), tft.color565(163, 148, 128)); //设置字体颜色和背景颜色
    spr.drawString(str_num, FONT_LEFT_START + 123, 32 + 1.5 * FONT_ROW_HEIGHT, 2);
    spr.drawString(str_conf, FONT_LEFT_START + 80, 32 + 3.5 * FONT_ROW_HEIGHT, 2);
    spr.drawString(str_light, FONT_LEFT_START + 186, 32 + 1.5 * FONT_ROW_HEIGHT, 2);
    spr.drawString(str_mic_val, FONT_LEFT_START + 194, 32 + 2.5 * FONT_ROW_HEIGHT, 2);
    spr.drawString(str_x_values, FONT_LEFT_START + 179, 32 + 3.5 * FONT_ROW_HEIGHT, 2);
    spr.drawString(str_y_value, FONT_LEFT_START + 214, 32 + 3.5 * FONT_ROW_HEIGHT, 2);
    spr.drawString(str_z_val, FONT_LEFT_START + 254, 32 + 3.5 * FONT_ROW_HEIGHT, 2);

    spr.setTextColor(TFT_BLACK); //设置字体颜色和背景颜色
    spr.drawString(",", FONT_LEFT_START + 210, 32 + 3.5 * FONT_ROW_HEIGHT, 2);
    spr.drawString(",", FONT_LEFT_START + 250, 32 + 3.5 * FONT_ROW_HEIGHT, 2);

    spr.setTextColor(TFT_WHITE); //设置字体颜色和背景颜色
    spr.drawString("Please login to                    to", FONT_LEFT_START + 30, 32 + 6.5 * FONT_ROW_HEIGHT, 2);
    spr.drawString("bind your device.", FONT_LEFT_START + 30, 32 + 7.5 * FONT_ROW_HEIGHT, 2);

    spr.setTextColor(tft.color565(127, 255, 0)); //设置字体颜色和背景颜色
    spr.drawString("sensecap.seeed.cc", FONT_LEFT_START + 129, 32 + 6.5 * FONT_ROW_HEIGHT, 2);

    spr.pushSprite(0 + SCREEN_PADDING, 0 + SCREEN_PADDING);
    spr.deleteSprite();
}
void loop()
{

    Page_1();
    delay(200);
}
