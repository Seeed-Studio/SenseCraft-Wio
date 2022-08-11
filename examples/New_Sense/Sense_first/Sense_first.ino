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

struct _data_base
{
    String value;
    String type;
};

struct _STRUCT_INPUT_DATA
{
    _data_base data[4];
} GG;

void _main_display(int _CHOOSE, _STRUCT_INPUT_DATA KK,bool complex=0)
{

    spr.createSprite(90, 100);
    spr.fillRect(0, 0, 90, 100, tft.color565(0, 139, 0));
    spr.setFreeFont(FSS9);
    spr.setTextColor(TFT_GREEN);

    short int _SIZE;
    if(complex){
      _SIZE = 2;
    }
    else{
      _SIZE = 4;
    }
 
    for (int i = 0; i < 4; i++)
    {
        spr.drawString(KK.data[i].value, 2, 5 + 24 * i, _SIZE);
        spr.drawString(KK.data[i].type, 68, 5 + 24 * i, 2);
    }

    switch (_CHOOSE)
    {
    case 0:
        spr.pushSprite(20, 90);
        break;
    case 1:
        spr.pushSprite(120, 90);
        break;
    case 2:
        spr.pushSprite(220, 90);
        break;
    default:
        break;
    }

    spr.deleteSprite();
}

void _sub_title(String value)
{
    spr.createSprite(320, 15);
    spr.setFreeFont(FSSB9);
    spr.setTextColor(TFT_WHITE, tft.color565(100, 100, 100));
    spr.drawString(value, 120, 0, GFXFF);
    spr.pushSprite(0, 65);
    spr.deleteSprite();
}
void DISPLAY_INIT() // Display initialization, black background rotation
{
    tft.begin();
    tft.setRotation(3);
    tft.fillScreen(TFT_BLACK);
}

// 320*70 = 22400
void Sense_Display(int CHOOSE_PAGE) // Sense interface display
{
    spr.createSprite(320, 60);
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

void Network_state(int s_key)
{
    //
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

void Below_Right_State_Content(int gg_state) // SD 插拔状态 Grove 插拔状态
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
    DISPLAY_INIT();
    pinMode(WIO_MIC, INPUT);
    pinMode(WIO_LIGHT, INPUT);
    lis.begin(Wire1);
    lis.setOutputDataRate(LIS3DHTR_DATARATE_25HZ);
    lis.setFullScaleRange(LIS3DHTR_RANGE_2G);
}

int gg = 0;

void loop()
{
    Network_state(1);
    Sense_Display(0);
    gg++;

    _STRUCT_INPUT_DATA GG;

    //    for(int i = 0 ;i < 4; i++){
    //        GG.data[i].value = "AAAA";
    //    }

    GG.data[0].value = "A";
    GG.data[0].type = "'C";

    GG.data[1].value = "AA";
    GG.data[1].type = "kg";

    GG.data[2].value = "AAA";
    GG.data[2].type = "cd";

    GG.data[3].value = "AAAA";
    GG.data[3].type = "m/s";

    _main_display(0, GG,1);
    _main_display(1, GG);
    _main_display(2, GG);
    _sub_title("ABCDEFG");

    Below_Right_State_Content(1);
    delay(500);
}
