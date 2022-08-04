#include "ui.h"

UI::UI(Message &m)
    : Thread("UIThread", 256, 1),
      Mail(m)
{
    Start();
};

void UI::init()
{
    tft.begin();
    tft.setRotation(3);
    tft.fillScreen(TFT_BLACK);

    spr.createSprite(SCREEN_WIDTH, SCREEN_HIGH);
}

void UI::Run(){
    uint8_t nums;

    while (true) {
      nums = Mail.Receive(&buff, 256, 0);
      if (nums > 0){
        Serial.printf("Receive: %d ", nums);
        Serial.println(buff[0]);
      }
      else
        Serial.println("Receive: 0");
        Delay(Ticks::MsToTicks(100));
    }
}