#include "ui.h"

UI::UI(TFT_eSPI &lcd, TFT_eSprite &display, Message &m)
    : Thread("UIThread", 4096, 1),
      tft(lcd),
      spr(display),
      Mail(m)
{
    Start();
};

void UI::init()
{
}

void UI::Run()
{
    uint8_t nums;

    init();

    while (true)
    {
        nums = Mail.Receive(&buff, 256, 0);
        if (nums > 0)
        {
            Serial.printf("Receive: %d ", nums);
            Serial.println(buff[0]);
        }
        else
        {
            for (int i = 0; i < 2; i++)
            {
                (this->*page[i])();
                Delay(Ticks::MsToTicks(100));
            }
        }
    }
}

#define PIXEL 4 // Width of one letter
#define LEFT_SIDE 70
#define HIGHT_SIDE 47
const static unsigned int FONT_ROW_HEIGHT = 22; // The height of a letter

void UI::sense_1()
{
    spr.createSprite(SCREEN_WIDTH, SCREEN_HIGH);

    // put your main code here
    spr.setFreeFont(FSSB9);
    spr.setTextColor(TFT_BLACK, TFT_WHITE);

    spr.fillRect(4 * PIXEL, 0, 21 * PIXEL, FONT_ROW_HEIGHT + 15, tft.color565(135, 206, 235));
    spr.fillRect(30 * PIXEL, 0, 21 * PIXEL, FONT_ROW_HEIGHT + 15, TFT_WHITE);
    spr.fillRect(56 * PIXEL, 0, 21 * PIXEL, FONT_ROW_HEIGHT + 15, TFT_WHITE);

    spr.drawString("Process", 127, 11, GFXFF);
    spr.drawString("Network", 231, 11, GFXFF);

    spr.setTextColor(TFT_BLACK, tft.color565(135, 206, 235));
    spr.drawString("Sense", 32, 11, GFXFF);

    spr.drawLine(0, 2 * FONT_ROW_HEIGHT, SCREEN_WIDTH, 2 * FONT_ROW_HEIGHT, TFT_WHITE);

    spr.setFreeFont(FSS9);
    spr.setTextColor(TFT_BLACK, tft.color565(220, 220, 220));
    spr.fillRect(128, 50, 16 * PIXEL, FONT_ROW_HEIGHT, tft.color565(220, 220, 220));
    spr.drawString("Sensor", 132, 54, GFXFF);

    spr.setTextColor(TFT_WHITE, TFT_BLACK);
    spr.drawString("Light", 100 - LEFT_SIDE, 80, GFXFF);
    spr.drawString("Sound", 209 - LEFT_SIDE, 80, GFXFF);
    spr.drawString("IMU", 316 - LEFT_SIDE, 80, GFXFF);

    spr.setFreeFont(FSS24);
    spr.drawString("77", 20, 115, GFXFF);   // Display the value of the light sensor
    spr.drawString("356", 125, 115, GFXFF); // Display the value of loudness
    spr.setFreeFont(FSS12);
    spr.drawString("0.10", 241, 102, GFXFF);  // Display the value of IMU X-axis
    spr.drawString("-0.23", 241, 126, GFXFF); // Display the value of IMU Y-axis
    spr.drawString("-2.33", 241, 150, GFXFF); // Display the value of IMU Z-axis
    spr.setFreeFont(FSS9);
    spr.drawString("X,Y,Z", 316 - LEFT_SIDE, 174, GFXFF);

    spr.fillCircle(146, 200, 3, tft.color565(0, 193, 255));
    spr.fillCircle(163, 200, 3, tft.color565(220, 220, 220));

    spr.drawString("Network :", 5, 218, GFXFF);

    //  spr.setTextColor(TFT_RED, TFT_BLACK);             //Networking status indication：OFF
    //  spr.drawString("OFF", 82, 218 , GFXFF);

    spr.setTextColor(TFT_GREEN, TFT_BLACK);           // Networking status indication：ON
    spr.drawString("LoRa(SenseCAP)", 82, 218, GFXFF); // Show the network you are in

    spr.pushSprite(0, 0);
    spr.deleteSprite();

    Serial.println("Sense: 1");
}

void UI::sense_2()
{

    spr.createSprite(SCREEN_WIDTH, SCREEN_HIGH);

    // put your main code here
    spr.setFreeFont(FSSB9);
    spr.setTextColor(TFT_BLACK, TFT_WHITE);

    spr.fillRect(4 * PIXEL, 0, 21 * PIXEL, FONT_ROW_HEIGHT + 15, tft.color565(135, 206, 235));
    spr.fillRect(30 * PIXEL, 0, 21 * PIXEL, FONT_ROW_HEIGHT + 15, TFT_WHITE);
    spr.fillRect(56 * PIXEL, 0, 21 * PIXEL, FONT_ROW_HEIGHT + 15, TFT_WHITE);

    spr.drawString("Process", 127, 11, GFXFF);
    spr.drawString("Network", 231, 11, GFXFF);

    spr.setTextColor(TFT_BLACK, tft.color565(135, 206, 235));
    spr.drawString("Sense", 32, 11, GFXFF);

    spr.drawLine(0, 2 * FONT_ROW_HEIGHT, SCREEN_WIDTH, 2 * FONT_ROW_HEIGHT, TFT_WHITE);

    spr.setFreeFont(FSS9);
    spr.setTextColor(TFT_BLACK, tft.color565(220, 220, 220));
    spr.fillRect(128, 50, 16 * PIXEL, FONT_ROW_HEIGHT, tft.color565(220, 220, 220));
    spr.drawString("Sensor", 132, 54, GFXFF);

    spr.setTextColor(TFT_WHITE, TFT_BLACK);
    spr.drawString("Sound", 100 - LEFT_SIDE, 80, GFXFF);
    spr.drawString("IMU", 209 - LEFT_SIDE, 80, GFXFF);
    spr.drawString("ADD", 316 - LEFT_SIDE, 80, GFXFF);

    spr.setFreeFont(FSS24);
    spr.drawString("356", 20, 115, GFXFF); // Display the value of loudness
    spr.setFreeFont(FSS12);
    spr.drawString("0.10", 205 - LEFT_SIDE, 102, GFXFF);  // Display the value of IMU X-axis
    spr.drawString("-0.23", 205 - LEFT_SIDE, 126, GFXFF); // Display the value of IMU Y-axis
    spr.drawString("-2.33", 205 - LEFT_SIDE, 150, GFXFF); // Display the value of IMU Z-axis
    spr.setFreeFont(FSS9);
    spr.drawString("X,Y,Z", 205 - LEFT_SIDE, 174, GFXFF);

    spr.fillCircle(163, 200, 3, tft.color565(0, 193, 255));
    spr.fillCircle(146, 200, 3, tft.color565(220, 220, 220));

    spr.fillRect(244, 117, 40, 40, TFT_WHITE);
    spr.fillRect(263, 123, 3, 26, TFT_BLACK);
    spr.fillRect(251, 135, 26, 3, TFT_BLACK);

    spr.drawString("Network :", 5, 218, GFXFF);

    //  spr.setTextColor(TFT_RED, TFT_BLACK);                      //Networking status indication：OFF
    //  spr.drawString("OFF", 82, 218 , GFXFF);

    spr.setTextColor(TFT_GREEN, TFT_BLACK);           // Networking status indication：ON
    spr.drawString("LoRa(SenseCAP)", 82, 218, GFXFF); // Show the network you are in

    spr.pushSprite(0, 0);
    spr.deleteSprite();

    Serial.println("Sense: 2");
}