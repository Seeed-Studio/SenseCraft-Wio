#include <TFT_eSPI.h>
#include "Free_Fonts.h"
#include "LIS3DHTR.h"

LIS3DHTR<TwoWire> lis;

TFT_eSPI tft;
TFT_eSprite spr = TFT_eSprite(&tft);

#define SCREEN_WIDTH 320 // Wio Terminal Maximum Width
#define SCREEN_HIGH 240  // Wio Terminal Maximum Height
#define PIXEL 4          // Width of one letter
#define LEFT_SIDE 70
#define HIGHT_SIDE 47
const static unsigned int FONT_ROW_HEIGHT = 22; // The height of a letter

void DISPLAY_INIT() // Display initialization, black background rotation
{
    tft.begin();
    tft.setRotation(3);
    tft.fillScreen(TFT_BLACK);
}

void top(int _CHOOSE)
{
    spr.createSprite(SCREEN_WIDTH, 2 * FONT_ROW_HEIGHT + 1);

    if (_CHOOSE == 1)
        spr.fillRect(4 * PIXEL, 0, 21 * PIXEL, FONT_ROW_HEIGHT + 15, tft.color565(135, 206, 235));
    else
        spr.fillRect(4 * PIXEL, 0, 21 * PIXEL, FONT_ROW_HEIGHT + 15, TFT_WHITE);
    if (_CHOOSE == 2)
        spr.fillRect(30 * PIXEL, 0, 21 * PIXEL, FONT_ROW_HEIGHT + 15, tft.color565(135, 206, 235));
    else
        spr.fillRect(30 * PIXEL, 0, 21 * PIXEL, FONT_ROW_HEIGHT + 15, TFT_WHITE);
    if (_CHOOSE == 3)
        spr.fillRect(56 * PIXEL, 0, 21 * PIXEL, FONT_ROW_HEIGHT + 15, tft.color565(135, 206, 235));
    else
        spr.fillRect(56 * PIXEL, 0, 21 * PIXEL, FONT_ROW_HEIGHT + 15, TFT_WHITE);

    spr.setFreeFont(FSSB9);
    spr.setTextColor(TFT_BLACK);
    spr.drawString("Sense", 32, 11, GFXFF);
    spr.drawString("Process", 127, 11, GFXFF);
    spr.drawString("Network", 231, 11, GFXFF);

    spr.drawLine(0, 2 * FONT_ROW_HEIGHT, SCREEN_WIDTH, 2 * FONT_ROW_HEIGHT, TFT_WHITE);

    spr.pushSprite(0, 0);
    spr.deleteSprite();
}

void TFCard_IMU_Display()
{
    spr.createSprite(280, 3.7 * FONT_ROW_HEIGHT);
    // spr.fillRect(0, 0, 280, 3.7 * FONT_ROW_HEIGHT, tft.color565(100, 100, 100));
    spr.setFreeFont(FSSB9);
    spr.setTextColor(TFT_WHITE);
    spr.fillRect(34 * PIXEL, 0, 10 * PIXEL, FONT_ROW_HEIGHT, tft.color565(100, 100, 100));
    spr.drawString("IMU", 34 * PIXEL + 3, 2, GFXFF);

    spr.drawString("Save to TF card", 12 * PIXEL, 55, GFXFF);

    spr.fillCircle(52 * PIXEL, 63, 4, tft.color565(211, 211, 211));
    spr.fillRect(210, 59, 28, 9, tft.color565(211, 211, 211));
    spr.fillCircle(236, 63, 8, tft.color565(65, 105, 235));

    //        spr.fillCircle(52 * PIXEL, 63, 8, tft.color565(211, 211, 211));
    //        spr.fillCircle(236 , 63    , 4, tft.color565(211, 211, 211));
    //        spr.fillRect(210, 59, 28, 9, tft.color565(211, 211, 211));

    spr.pushSprite(2 * PIXEL, 2.2 * FONT_ROW_HEIGHT);
    spr.deleteSprite();
}

void Network_state(int s_key)
{
    spr.createSprite(SCREEN_WIDTH, 25);
    spr.setFreeFont(FSSB9);
    spr.fillSprite(TFT_BLACK);

    switch (s_key)
    {
    case 0:
        spr.setTextColor(TFT_RED);
        spr.drawString("OFF", 90, 0, 2);
        break;
    case 1:
        spr.setTextColor(TFT_GREEN, TFT_BLACK); // Networking status indication：ON
        spr.drawString("LoRa", 60, 0, 2);       // Show the network you are in
        break;
    case 2:
        spr.setTextColor(TFT_GREEN, TFT_BLACK); // Networking status indication：ON
        spr.drawString("WIFI", 60, 0, 2);       // Show the network you are in
        break;
    default:;
    }
    spr.setTextColor(TFT_WHITE);
    spr.drawString("Network:", 5, 0, 2);
    spr.setFreeFont(FSS9);

    spr.pushSprite(0, 215);
    spr.deleteSprite();
}

void Grove_Tip()
{
    spr.createSprite(SCREEN_WIDTH, 25);
    spr.fillSprite(TFT_BLACK);
    spr.setFreeFont(FSS9);
    spr.setTextColor(TFT_YELLOW);
    spr.drawString("Plug in a Grove sensor", 0, 0, 2);
    spr.pushSprite(170, 215);
    spr.deleteSprite();
}

void TFcard_Tip()
{
    spr.createSprite(SCREEN_WIDTH, 25);
    spr.setFreeFont(FSS9);
    spr.drawTriangle(0, 18, 11, 0, 22, 18, TFT_YELLOW);
    spr.setTextColor(TFT_YELLOW);
    spr.drawString("! ", 10, 4, 2);
    spr.drawString("TF card storage is full", 28, 2, 2);
    spr.pushSprite(140, 215);
    spr.deleteSprite();
}

void setup()
{
    DISPLAY_INIT();
    pinMode(WIO_MIC, INPUT);
    lis.begin(Wire1);
    lis.setOutputDataRate(LIS3DHTR_DATARATE_25HZ);
    lis.setFullScaleRange(LIS3DHTR_RANGE_2G);
}

void loop()
{
    Network_state(1);
    top(1);

    TFCard_IMU_Display();
    // Grove_Tip();
    TFcard_Tip();
    delay(500);
}
