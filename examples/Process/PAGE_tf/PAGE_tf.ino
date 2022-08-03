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

int gg_switch = 0;
int gg_switch_k = 1;
int gg_switch_circle = 10;                        // r 外部大圆半径
int gg_switch_rect_width = 20;                    // width x++ 矩形宽度
int gg_switch_rect_high = 1;                      // high  y++
int gg_switch_location_x = FONT_LEFT_START + 260; // location 左侧圆心
int gg_switch_location_y = 115;
int gg_switch_state = 0; // 0 off 1 on
unsigned int gg_switch_state_color[4] = {tft.color565(211, 211, 211), TFT_BLUE, tft.color565(201, 201, 201), tft.color565(65, 105, 235)};

void switch_button_gui(int gg_switch_location_x, int gg_switch_location_y, int gg_switch_circle, int gg_switch_rect_width, int gg_switch)
{
    if (gg_switch == 1)
    {
        spr.fillCircle(gg_switch_location_x + gg_switch_rect_width, gg_switch_location_y, gg_switch_circle, gg_switch_state_color[gg_switch]);
        spr.fillCircle(gg_switch_location_x, gg_switch_location_y, gg_switch_circle / 2, gg_switch_state_color[gg_switch + 2]);
        spr.fillRect(gg_switch_location_x, gg_switch_location_y - gg_switch_circle / 2, gg_switch_rect_width, gg_switch_circle + 1, gg_switch_state_color[gg_switch + 2]);
    }
    else
    {
        spr.fillCircle(gg_switch_location_x, gg_switch_location_y, gg_switch_circle, gg_switch_state_color[gg_switch]);
        spr.fillCircle(gg_switch_location_x + gg_switch_rect_width, gg_switch_location_y, gg_switch_circle / 2, gg_switch_state_color[gg_switch + 2]);
        spr.fillRect(gg_switch_location_x, gg_switch_location_y - gg_switch_circle / 2, gg_switch_rect_width, gg_switch_circle + 1, gg_switch_state_color[gg_switch + 2]);
    }
}

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
    DISPLAY_INIT();
    pinMode(WIO_KEY_A, INPUT_PULLUP);
    pinMode(WIO_KEY_B, INPUT_PULLUP);
    pinMode(WIO_KEY_C, INPUT_PULLUP);
}
int gg_storage_is_full = 0;
void Page_tf()
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

    Sense_Display();
    spr.setTextColor(TFT_WHITE);
    spr.drawString("Save to TF card ", 40, gg_switch_location_y - gg_switch_circle, FONT4);

    if (digitalRead(WIO_KEY_A) == LOW)
    {
        Serial.println("A Key pressed");
        gg_switch_k *= -1;
        delay(200);
    }
    if (digitalRead(WIO_KEY_B) == LOW)
    {
        Serial.println("B Key pressed");
        gg_storage_is_full++;
        delay(200);
    }

    if (gg_switch_k > 0)
    {
        gg_switch = 1;
    }
    else
    {
        gg_switch = 0;
    }

    switch_button_gui(gg_switch_location_x, gg_switch_location_y, gg_switch_circle, gg_switch_rect_width, gg_switch);
    //    int gg_storage_is_full = 1 ;

    if (gg_storage_is_full % 2)
    {
        spr.setTextColor(TFT_YELLOW);
        spr.setFreeFont(FSSB9);
        spr.drawTriangle(148, 155, 159, 132, 170, 155, TFT_YELLOW);
        spr.drawString("! ", 158, 140, 2);
        spr.drawString("TF card storage space is fully occupied ", 30, 160, 2);
    }

    else
    {
        if (gg_switch == 0)
        {
            spr.setTextColor(TFT_YELLOW);
            spr.setFreeFont(FSSB9);
            spr.drawString("Please insert TF card to activate this function ", 10, 140, 2);
        }
        else
        {
            spr.fillRect(40, 130, 250, 50, TFT_WHITE);
            spr.setTextColor(TFT_BLACK);
            spr.setFreeFont(FSSB9);
            spr.drawString("Saving has been started", 60, 140, GFXFF);
        }
    }

    spr.setTextColor(TFT_YELLOW);
    spr.drawString("Network:", 10, 220, GFXFF);
    spr.setTextColor(tft.color565(254, 0, 0));
    spr.drawString("OFF", 100, 220, GFXFF);

    spr.pushSprite(0 + SCREEN_PADDING, 0 + SCREEN_PADDING);
    spr.deleteSprite();
}
void loop()
{
    Page_tf();
    delay(200);
}
