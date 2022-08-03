#include <TFT_eSPI.h>
#include "Free_Fonts.h"

TFT_eSPI tft;
TFT_eSprite spr = TFT_eSprite(&tft);

#define SCREEN_WIDTH 320                        // Wio Terminal Maximum Width
#define SCREEN_HIGH 240                         // Wio Terminal Maximum Height
#define PIXEL 4                                 // Width of one letter
#define MOVE_PIXEL_y 20                         // Virtual to Reality Moving Distance
const static unsigned int FONT_ROW_HEIGHT = 22; // The height of a letter

void DISPLAY_INIT() // Display initialization, black background rotation
{
    tft.begin();
    tft.setRotation(3);
    tft.fillScreen(TFT_BLACK);
}

void Process_main(int page)
{
    switch (page)
    {

    case 1:
        spr.setTextColor(TFT_WHITE);
        spr.drawString("Vision AI real-time analysis", 74, 33 + MOVE_PIXEL_y, GFXFF);
        spr.fillRect(24, 71 + MOVE_PIXEL_y - 3, 221, FONT_ROW_HEIGHT, tft.color565(0, 204, 2));
        break;
    case 2:
        spr.setTextColor(TFT_WHITE);
        spr.drawString("TinyML Example", 107, 32 + MOVE_PIXEL_y, GFXFF);
        spr.fillRect(24, 100 + MOVE_PIXEL_y - 3, 135, FONT_ROW_HEIGHT, tft.color565(0, 204, 2));
        break;
    default:;
    }
    spr.setFreeFont(FSS9);
    spr.setTextColor(TFT_WHITE);
    spr.drawString("Vision AI real-time analysis", 24, 71 + MOVE_PIXEL_y, GFXFF);
    spr.drawString("TinyML Example", 24, 100 + MOVE_PIXEL_y, GFXFF);

    spr.setTextColor(tft.color565(169, 169, 169));
    spr.drawString("Data Filter(In Development)", 24, 129 + MOVE_PIXEL_y, GFXFF);
}

void Network_state(int s_key)
{
    switch (s_key)
    {
    case 0:
        spr.setTextColor(TFT_RED);
        spr.drawString("Off", 80, 215, GFXFF);
        break;
    case 1:
        spr.setTextColor(TFT_GREEN);
        spr.drawString("On", 80, 215, GFXFF);
        break;
    default:;
    }
    spr.setTextColor(TFT_WHITE);
    spr.drawString("Network:", 7, 215, GFXFF);
}

void Process_TinyML_ENTER(void)
{
    spr.setTextColor(TFT_WHITE);
    spr.drawString("Please scan the QR ", 135, 86, GFXFF);
    spr.drawString("code on the screen ", 135, 106, GFXFF);
    spr.drawString("to view the Github", 135, 126, GFXFF);
    spr.drawString("sample tutorial ", 135, 146, GFXFF);
}

void Vision_AI_real_time_analysis(void) //todo
{
    spr.setTextColor(TFT_WHITE);
    spr.drawString("Vision AI real-time analysis", 64, 52, GFXFF);
    spr.fillRect(40, 73, 216 + 20, 117 + 15, tft.color565(128, 128, 128));
}

void Sense_Display(int G) // Sense interface display
{
    spr.createSprite(SCREEN_WIDTH, SCREEN_HIGH);

    //  choose func  here
    //  Process_main();
    //    Process_TinyML_ENTER();
    Vision_AI_real_time_analysis();
    spr.setFreeFont(FSSB9);
    spr.setTextColor(TFT_BLACK, TFT_WHITE);

    spr.fillRect(4 * PIXEL, 0, 21 * PIXEL, FONT_ROW_HEIGHT + 15, TFT_WHITE);
    spr.fillRect(30 * PIXEL, 0, 21 * PIXEL, FONT_ROW_HEIGHT + 15, tft.color565(135, 206, 235));
    spr.fillRect(56 * PIXEL, 0, 21 * PIXEL, FONT_ROW_HEIGHT + 15, TFT_WHITE);

    spr.setFreeFont(FSS9);
    spr.setTextColor(TFT_BLACK);
    spr.drawString("Sense", 32, 11, GFXFF);
    spr.drawString("Process", 127, 11, GFXFF);
    spr.drawString("Network", 231, 11, GFXFF);

    spr.drawLine(0, 2 * FONT_ROW_HEIGHT, SCREEN_WIDTH, 2 * FONT_ROW_HEIGHT, TFT_WHITE);

    Network_state(1);

    spr.pushSprite(0, 0);
    spr.deleteSprite();
}

void setup()
{
    DISPLAY_INIT();
    pinMode(WIO_KEY_A, INPUT_PULLUP);
    pinMode(WIO_KEY_B, INPUT_PULLUP);
    pinMode(WIO_KEY_C, INPUT_PULLUP);
}

int gg_switch = 0;
int gg_switch_flag = 0;
void loop()
{
    // test block begin
    if (digitalRead(WIO_KEY_B) == LOW)
    {
        Serial.println("A Key pressed");
        gg_switch++;
        delay(200);
    }

    Sense_Display(gg_switch % 3);
}
