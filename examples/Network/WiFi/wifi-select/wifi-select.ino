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

void WifiSelection()
{
    spr.createSprite(SCREEN_WIDTH, SCREEN_HIGH);

    // put your main code here
    spr.setFreeFont(FSSB9);
    spr.setTextColor(TFT_BLACK, TFT_WHITE);

    spr.fillRect(4 * PIXEL, 0, 21 * PIXEL, FONT_ROW_HEIGHT + 15, TFT_WHITE);
    spr.fillRect(30 * PIXEL, 0, 21 * PIXEL, FONT_ROW_HEIGHT + 15, TFT_WHITE);
    spr.fillRect(56 * PIXEL, 0, 21 * PIXEL, FONT_ROW_HEIGHT + 15, tft.color565(135, 206, 235));
    spr.fillRect(30 * PIXEL, 8.0 * FONT_ROW_HEIGHT, 21 * PIXEL, FONT_ROW_HEIGHT + 15, tft.color565(70, 130, 160));

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
    spr.drawString("Please refer to the wiki to modify", 25, 75 + FONT_ROW_HEIGHT, GFXFF);
    spr.drawString("the configuration file and send it", 25, 75 + 2 * FONT_ROW_HEIGHT, GFXFF);
    spr.drawString("to this device.", 25, 75 + 3 * FONT_ROW_HEIGHT, GFXFF);

    spr.setFreeFont(FSS12);
    spr.drawString("OK", 36 * PIXEL, 8.4 * FONT_ROW_HEIGHT, GFXFF);

    spr.setFreeFont(FSS9);
    spr.drawString("Network :", 5, 218, GFXFF);

    spr.setTextColor(TFT_RED, TFT_BLACK); // Networking status indication：OFF
    spr.drawString("OFF", 82, 218, GFXFF);

    //    spr.setTextColor(TFT_GREEN, TFT_BLACK);             //Networking status indication：ON
    //    spr.drawString("WiFi(Ubidots)", 82, 218 , GFXFF);  //Show the network you are in

    spr.pushSprite(0, 0);
    spr.deleteSprite();
}

void WifiWait_Display()
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

    spr.setFreeFont(FSS9);

    spr.setTextColor(TFT_WHITE, tft.color565(112, 112, 112));
    spr.drawString("WIFI(Ubidots)", 102, 48, GFXFF);
    spr.setTextColor(TFT_WHITE);
    spr.drawString("Waiting for configuration...", 55, 75 + 2.5 * FONT_ROW_HEIGHT, GFXFF);

    spr.setFreeFont(FSS9);
    spr.drawString("Network :", 5, 218, GFXFF);

    spr.setTextColor(TFT_RED, TFT_BLACK); // Networking status indication：OFF
    spr.drawString("OFF", 82, 218, GFXFF);

    //    spr.setTextColor(TFT_GREEN, TFT_BLACK);             //Networking status indication：ON
    //    spr.drawString("WiFi(Ubidots)", 82, 218 , GFXFF);  //Show the network you are in

    spr.pushSprite(0, 0);
    spr.deleteSprite();
}

void Wificonnect_Display()
{
    spr.createSprite(SCREEN_WIDTH, SCREEN_HIGH);

    // put your main code here
    spr.setFreeFont(FSSB9);
    spr.setTextColor(TFT_BLACK, TFT_WHITE);

    spr.fillRect(4 * PIXEL, 0, 21 * PIXEL, FONT_ROW_HEIGHT + 15, TFT_WHITE);
    spr.fillRect(30 * PIXEL, 0, 21 * PIXEL, FONT_ROW_HEIGHT + 15, TFT_WHITE);
    spr.fillRect(56 * PIXEL, 0, 21 * PIXEL, FONT_ROW_HEIGHT + 15, tft.color565(135, 206, 235));
    spr.fillRect(30 * PIXEL, 8.0 * FONT_ROW_HEIGHT, 21 * PIXEL, FONT_ROW_HEIGHT + 15, tft.color565(70, 130, 160));

    spr.drawString("Sense", 32, 11, GFXFF);
    spr.drawString("Process", 127, 11, GFXFF);

    spr.setTextColor(TFT_BLACK);
    spr.drawString("Network", 231, 11, GFXFF);

    spr.drawLine(0, 2 * FONT_ROW_HEIGHT, SCREEN_WIDTH, 2 * FONT_ROW_HEIGHT, TFT_WHITE);
    // spr.drawLine(0, 3.5 * FONT_ROW_HEIGHT, SCREEN_WIDTH, 3.5 * FONT_ROW_HEIGHT, TFT_WHITE);

    spr.setFreeFont(FSS9);

    spr.setTextColor(TFT_WHITE, tft.color565(112, 112, 112));
    spr.drawString("WIFI(Ubidots)", 102, 48, GFXFF);

    spr.setTextColor(TFT_WHITE);
    spr.drawString("Successful!", 115, 75 + 2 * FONT_ROW_HEIGHT, GFXFF);

    spr.setFreeFont(FSS12);
    spr.drawString("OK", 36 * PIXEL, 8.4 * FONT_ROW_HEIGHT, GFXFF);

    spr.setFreeFont(FSS9);
    spr.drawString("Network :", 5, 218, GFXFF);

    //    spr.setTextColor(TFT_RED, TFT_BLACK);             //Networking status indication：OFF
    //    spr.drawString("OFF", 82, 218 , GFXFF);

    spr.setTextColor(TFT_GREEN, TFT_BLACK);          // Networking status indication：ON
    spr.drawString("WiFi(Ubidots)", 82, 218, GFXFF); // Show the network you are in

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

void loop()
{
    /// WifiSelection()请使用 Network Select中的实现 点击lora目录网络选择的那个ino就可以看到 我写readme里面 或者一会加上 太多了我先实现新界面 https://github.com/Seeed-Studio/Seeed_Arduino_K1100/commit/c1501d156564bb6538af02114e0d336db666e02e
    //    WifiSelection();   // Repeat in Network Select LORA_WIFI select network step
    //    delay(2000);

    ///  WifiWait_Display(); wifi等待的界面 需要使用的
    //    WifiWait_Display(); //ok
    //    delay(2000);

    // Wificonnect_Display(); //WIFI 连接成功的界面 需要使用
    Wificonnect_Display();
    delay(2000);
}
