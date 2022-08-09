#include <TFT_eSPI.h>
#include "Free_Fonts.h"

TFT_eSPI tft;
TFT_eSprite spr = TFT_eSprite(&tft);

#define SCREEN_WIDTH 320                        // Wio Terminal Maximum Width
#define SCREEN_HIGH 240                         // Wio Terminal Maximum Height
#define PIXEL 4                                 // Width of one letter
const static unsigned int FONT_ROW_HEIGHT = 22; // The height of a letter

// Key
int key_status = 0;

void key() //设置按键
{
    if (digitalRead(WIO_KEY_C) == LOW)
    {
        Serial.println("A Key pressed");
        key_status = 1;
    }
    else if (digitalRead(WIO_KEY_B) == LOW)
    {
        Serial.println("B Key pressed");
        key_status = 2;
    }
    else if (digitalRead(WIO_KEY_A) == LOW)
    {
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

void NetworkHome_Display(int network_select_status) // Select network interface 1 Lora 2 wifi 0 未选择状态
{
    spr.createSprite(SCREEN_WIDTH, SCREEN_HIGH);

    // put your main code here
    spr.setFreeFont(FSSB9);
    spr.setTextColor(TFT_BLACK, TFT_WHITE);

    spr.fillRect(4 * PIXEL, 0, 21 * PIXEL, FONT_ROW_HEIGHT + 15, TFT_WHITE);
    spr.fillRect(30 * PIXEL, 0, 21 * PIXEL, FONT_ROW_HEIGHT + 15, TFT_WHITE);
    spr.fillRect(56 * PIXEL, 0, 21 * PIXEL, FONT_ROW_HEIGHT + 15, tft.color565(135, 206, 235));

    spr.drawString("Sense", 32, 11, GFXFF);
    spr.drawString("Process", 127, 11, GFXFF);

    spr.setTextColor(TFT_BLACK);
    spr.drawString("Network", 231, 11, GFXFF);

    spr.drawLine(0, 2 * FONT_ROW_HEIGHT, SCREEN_WIDTH, 2 * FONT_ROW_HEIGHT, TFT_WHITE);
    // spr.drawLine(0, 3.5 * FONT_ROW_HEIGHT, SCREEN_WIDTH, 3.5 * FONT_ROW_HEIGHT, TFT_WHITE);
    //    int network_select_status=0; //For debug
    switch (network_select_status)
    {
    case 1:
        spr.setTextColor(TFT_WHITE, tft.color565(112, 112, 112));
        spr.drawString("LoRa(SenseCAP)", 92, 48, GFXFF);
        spr.fillRect(30, 85, 145, 45, TFT_GREEN);
        spr.setTextColor(TFT_WHITE, TFT_BLACK);
        spr.drawString("Please toggle the bottom right", 30, 95 + FONT_ROW_HEIGHT + 20, GFXFF);
        spr.drawString("button left and right to select", 30, 95 + 2 * FONT_ROW_HEIGHT + 20, GFXFF);
        spr.drawString("the network.", 30, 95 + 3 * FONT_ROW_HEIGHT + 20, GFXFF);

        break;
    case 2:

        spr.setTextColor(TFT_WHITE, tft.color565(112, 112, 112));
        spr.drawString("WIFI(Ubidots)", 102, 48, GFXFF);
        spr.fillRect(206, 85, 78, 45, TFT_GREEN);

        spr.setTextColor(TFT_WHITE, TFT_BLACK);
        spr.drawString("Please refer to the wiki to", 30, 95 + FONT_ROW_HEIGHT + 20, GFXFF);
        spr.drawString("modify the configuration file", 30, 95 + 2 * FONT_ROW_HEIGHT + 20, GFXFF);
        spr.drawString("and send it to this device.", 30, 95 + 3 * FONT_ROW_HEIGHT + 20, GFXFF);

        break;
    default:
        break;
    }

    spr.setFreeFont(FSS9);
    spr.setTextColor(TFT_WHITE);
    spr.drawString("LoRa", 32, 88, GFXFF);
    spr.drawString("LoRa(SenseCAP)", 32, 108, GFXFF);
    spr.drawString("WIFI", 208, 88, GFXFF);
    spr.drawString("(Ubidots)", 208, 108, GFXFF);

    //    spr.setTextColor(TFT_WHITE, TFT_BLACK);
    //    spr.drawString("Please toggle the bottom right", 30, 95 + FONT_ROW_HEIGHT + 20, GFXFF);
    //    spr.drawString("button left and right to select", 30, 95 + 2 * FONT_ROW_HEIGHT + 20, GFXFF);
    //    spr.drawString("the network.", 30, 95 + 3 * FONT_ROW_HEIGHT + 20, GFXFF);

    spr.drawString("Network :", 5, 218, GFXFF);

    spr.setTextColor(TFT_RED, TFT_BLACK); // Networking status indication：OFF
    spr.drawString("OFF", 82, 218, GFXFF);

    spr.pushSprite(0, 0);
    spr.deleteSprite();
}

void NetSelection(int key)
{
    spr.createSprite(SCREEN_WIDTH, SCREEN_HIGH);

    // put your main code here
    spr.setFreeFont(FSSB9);
    spr.setTextColor(TFT_BLACK, TFT_WHITE);

    spr.fillRect(4 * PIXEL, 0, 21 * PIXEL, FONT_ROW_HEIGHT + 15, TFT_WHITE);
    spr.fillRect(30 * PIXEL, 0, 21 * PIXEL, FONT_ROW_HEIGHT + 15, TFT_WHITE);
    spr.fillRect(56 * PIXEL, 0, 21 * PIXEL, FONT_ROW_HEIGHT + 15, tft.color565(135, 206, 235));
    spr.fillRect(30 * PIXEL, 8.0 * FONT_ROW_HEIGHT, 21 * PIXEL, FONT_ROW_HEIGHT + 15, tft.color565(70, 130, 160));

    spr.fillRect(156, 54, 40, 4, tft.color565(220, 220, 220));

    spr.drawString("Sense", 32, 11, GFXFF);
    spr.drawString("Process", 127, 11, GFXFF);

    spr.setTextColor(TFT_BLACK);
    spr.drawString("Network", 231, 11, GFXFF);

    spr.drawLine(0, 2 * FONT_ROW_HEIGHT, SCREEN_WIDTH, 2 * FONT_ROW_HEIGHT, TFT_WHITE);
    // spr.drawLine(0, 3.5 * FONT_ROW_HEIGHT, SCREEN_WIDTH, 3.5 * FONT_ROW_HEIGHT, TFT_WHITE);

    spr.setFreeFont(FSS9);

    spr.setTextColor(TFT_WHITE, tft.color565(112, 112, 112));
    spr.drawString("LoRa(SenseCAP)", 92, 48, GFXFF);

    spr.setTextColor(TFT_WHITE);
    spr.drawString("Please press the bottom right", 35, 75 + FONT_ROW_HEIGHT, GFXFF);
    spr.drawString("button to confirm your network", 35, 75 + 2 * FONT_ROW_HEIGHT, GFXFF);
    spr.drawString("selection.", 35, 75 + 3 * FONT_ROW_HEIGHT, GFXFF);

    spr.setFreeFont(FSS12);
    spr.drawString("OK", 36 * PIXEL, 8.4 * FONT_ROW_HEIGHT, GFXFF);

    spr.setFreeFont(FSS9);
    spr.drawString("Network :", 5, 218, GFXFF);

    spr.setTextColor(TFT_RED, TFT_BLACK); // Networking status indication：OFF
    spr.drawString("OFF", 82, 218, GFXFF);

    //    spr.setTextColor(TFT_GREEN, TFT_BLACK);             //Networking status indication：ON
    //    spr.drawString("LoRa(SenseCAP)", 82, 218 , GFXFF);  //Show the network you are in

    spr.pushSprite(0, 0);
    spr.deleteSprite();
}

void WioE5connect_Display()
{
    spr.createSprite(SCREEN_WIDTH, SCREEN_HIGH);

    // put your main code here
    spr.setFreeFont(FSSB9);
    spr.setTextColor(TFT_BLACK, TFT_WHITE);

    spr.fillRect(4 * PIXEL, 0, 21 * PIXEL, FONT_ROW_HEIGHT + 15, TFT_WHITE);
    spr.fillRect(30 * PIXEL, 0, 21 * PIXEL, FONT_ROW_HEIGHT + 15, TFT_WHITE);
    spr.fillRect(56 * PIXEL, 0, 21 * PIXEL, FONT_ROW_HEIGHT + 15, tft.color565(135, 206, 235));
    spr.fillRect(30 * PIXEL, 8.0 * FONT_ROW_HEIGHT, 21 * PIXEL, FONT_ROW_HEIGHT + 15, tft.color565(70, 130, 160));

    spr.fillRect(156, 54, 40, 4, tft.color565(220, 220, 220));

    spr.drawString("Sense", 32, 11, GFXFF);
    spr.drawString("Process", 127, 11, GFXFF);

    spr.setTextColor(TFT_BLACK, tft.color565(135, 206, 235));
    spr.drawString("Network", 231, 11, GFXFF);

    spr.drawLine(0, 2 * FONT_ROW_HEIGHT, SCREEN_WIDTH, 2 * FONT_ROW_HEIGHT, TFT_WHITE);

    spr.setFreeFont(FSS9);

    spr.setTextColor(TFT_WHITE, tft.color565(112, 112, 112));
    spr.drawString("LoRa(SenseCAP)", 92, 48, GFXFF);

    //    spr.fillCircle(175, 56, 10, tft.color565(190, 190, 190));                 //Default in no network state selected

    spr.setTextColor(TFT_WHITE);
    spr.drawString("Please connect the Grove-Wio E5", 25, 75 + FONT_ROW_HEIGHT, GFXFF);
    spr.drawString("to the Grove connector on the", 25, 75 + 2 * FONT_ROW_HEIGHT, GFXFF);
    spr.drawString("bottom right side of the screen.", 25, 75 + 3 * FONT_ROW_HEIGHT, GFXFF);

    spr.setFreeFont(FSS12);
    spr.drawString("OK", 36 * PIXEL, 8.4 * FONT_ROW_HEIGHT, GFXFF);

    spr.setFreeFont(FSS9);
    spr.drawString("Network :", 5, 218, GFXFF);

    spr.setTextColor(TFT_RED, TFT_BLACK); // Networking status indication：OFF
    spr.drawString("OFF", 82, 218, GFXFF);

    //    spr.setTextColor(TFT_GREEN, TFT_BLACK);             //Networking status indication：ON
    //    spr.drawString("LoRa(SenseCAP)", 82, 218 , GFXFF);  //Show the network you are in

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

int gg = 0;
void loop()
{
    //    key();  //ABC button to select network
    gg++;
    NetworkHome_Display(gg % 3); // Select network interface 1 Lora 2 wifi 0 未选择状态
    delay(1000);
    //    NetSelection(key_status);
    //    delay(2000);
    //    WioE5connect_Display();
    //    delay(2000);
}
