#include <TFT_eSPI.h>
#include "Free_Fonts.h"

TFT_eSPI tft;
TFT_eSprite spr = TFT_eSprite(&tft);

#define SCREEN_WIDTH 320                        // Wio Terminal Maximum Width
#define SCREEN_HIGH 240                         // Wio Terminal Maximum Height
#define PIXEL 4                                 // Width of one letter
const static unsigned int FONT_ROW_HEIGHT = 22; // The height of a letter


//Key
int key_status = 0;

void key()                 //设置按键
{
    if (digitalRead(WIO_KEY_C) == LOW) {
      Serial.println("A Key pressed");
      key_status = 1;
     }
     else if (digitalRead(WIO_KEY_B) == LOW) {
      Serial.println("B Key pressed");
      key_status = 2;
     }
     else if (digitalRead(WIO_KEY_A) == LOW) {
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

void ConnectSuc_Display(int key) // Select Frequency band interface
{
    spr.createSprite(SCREEN_WIDTH, SCREEN_HIGH);

    // put your main code here
    spr.setFreeFont(FSSB9);
    spr.setTextColor(TFT_BLACK);

    spr.fillRect(4 * PIXEL, 0, 21 * PIXEL, FONT_ROW_HEIGHT + 15, TFT_WHITE);
    spr.fillRect(30 * PIXEL, 0, 21 * PIXEL, FONT_ROW_HEIGHT + 15, TFT_WHITE);
    spr.fillRect(56 * PIXEL, 0, 21 * PIXEL, FONT_ROW_HEIGHT + 15, tft.color565(135, 206, 235));

    spr.fillRect(156, 54, 40, 4, tft.color565(220, 220, 220));
    spr.fillRect(28 * PIXEL, 8.0 * FONT_ROW_HEIGHT, 25 * PIXEL, FONT_ROW_HEIGHT + 14, tft.color565(160, 34, 34));

    spr.fillCircle(160, 55, 10, TFT_BLUE);                                    //button to the left to select LoRa network

    spr.drawString("Sense", 32, 11, GFXFF);
    spr.drawString("Process", 127, 11, GFXFF);
    
    spr.setTextColor(TFT_BLACK);
    spr.drawString("Network", 231, 11, GFXFF);
  
    spr.drawLine(0, 3.5 * FONT_ROW_HEIGHT, SCREEN_WIDTH, 3.5 * FONT_ROW_HEIGHT, TFT_WHITE);
  
    spr.setFreeFont(FSS9);
    spr.setTextColor(TFT_WHITE, tft.color565(0, 139, 0));
    spr.drawString("   LoRa   ", 5, 48, GFXFF); //LoRa(SenseCAP)
    
    spr.setTextColor(TFT_WHITE, tft.color565(100, 100, 100));
    spr.drawString("   WiFi   ", 205, 48, GFXFF); //iFi(Ubidots)

    spr.setTextColor(TFT_WHITE);
    spr.drawString("Connected: LoRa        ", 30, 3.8 * FONT_ROW_HEIGHT, 2); //(SenseCAP)
    spr.drawString("Signal:", 30, 4.8 * FONT_ROW_HEIGHT, 2);
    spr.drawString("All data:", 30, 5.8 * FONT_ROW_HEIGHT, 2);
    spr.drawString("packets", 140, 5.8 * FONT_ROW_HEIGHT, 2);
    spr.drawString("Success:", 30, 6.8 * FONT_ROW_HEIGHT, 2);
    spr.drawString("packets", 140, 6.8 * FONT_ROW_HEIGHT, 2);

    spr.setFreeFont(FSSB9);
    spr.setTextColor(tft.color565(0, 139, 0));
    spr.drawString("10000", 90, 5.8 * FONT_ROW_HEIGHT, 2);   //Show total number of packages issued
    spr.drawString("999", 90, 6.8 * FONT_ROW_HEIGHT, 2);     //Shows the number of successful deliveries

//    spr.fillRect(78, 110, 3, 11, tft.color565(100, 100, 100));   //No signal
//    spr.fillRect(84, 107, 3, 14, tft.color565(100, 100, 100));   
//    spr.fillRect(90, 104, 3, 17, tft.color565(100, 100, 100));
//    spr.fillRect(96, 101, 3, 20, tft.color565(100, 100, 100));

    if(key == 0)  //Press the A button to disconnect
    {
      spr.fillRect(78, 110, 3, 11, tft.color565(0, 139, 0));            //Two-frame signal
      spr.fillRect(84, 107, 3, 14, tft.color565(0, 139, 0));   
      spr.fillRect(90, 104, 3, 17, tft.color565(100, 100, 100));
      spr.fillRect(96, 101, 3, 20, tft.color565(100, 100, 100));

      spr.fillCircle(265, 4.9 * FONT_ROW_HEIGHT, 10, TFT_GREEN);        //Data transmission status: normal
      spr.setTextColor(TFT_WHITE);
      spr.drawString("Send", 253, 5.8 * FONT_ROW_HEIGHT, 2);
      spr.drawString("Success", 245, 6.6 * FONT_ROW_HEIGHT, 2);

      spr.setFreeFont(FSS9);
      spr.setTextColor(tft.color565(0, 139, 0), TFT_BLACK);             //Networking status indication：ON
      spr.drawString("    LoRa    ", 82, 218 , GFXFF);  //(SenseCAP)
    }
    if(key == 1)  //Press the A button to disconnect
    {
      spr.fillRect(78, 110, 3, 11, tft.color565(0, 139, 0));            //Four-frame signal
      spr.fillRect(84, 107, 3, 14, tft.color565(0, 139, 0));   
      spr.fillRect(90, 104, 3, 17, tft.color565(0, 139, 0));
      spr.fillRect(96, 101, 3, 20, tft.color565(0, 139, 0));

      spr.fillCircle(265, 4.9 * FONT_ROW_HEIGHT, 10, TFT_GREEN);        //Data transmission status: normal
      spr.setTextColor(TFT_WHITE);
      spr.drawString("Send", 253, 5.8 * FONT_ROW_HEIGHT, 2);
      spr.drawString("Success", 245, 6.6 * FONT_ROW_HEIGHT, 2);

      spr.setFreeFont(FSS9);
      spr.setTextColor(tft.color565(0, 139, 0), TFT_BLACK);             //Networking status indication：ON
      spr.drawString("    LoRa    ", 82, 218 , GFXFF);  //LoRa(SenseCAP)
    }
    if(key == 2)  //Press the B button to disconnect
    {
      spr.fillRect(78, 110, 3, 11, tft.color565(0, 139, 0));            //One frame signal
      spr.fillRect(84, 107, 3, 14, tft.color565(100, 100, 100));   
      spr.fillRect(90, 104, 3, 17, tft.color565(100, 100, 100));
      spr.fillRect(96, 101, 3, 20, tft.color565(100, 100, 100));

      spr.fillCircle(265, 4.9 * FONT_ROW_HEIGHT, 10, tft.color565(255, 165, 0));  //Data transmission status: Packet loss
      spr.setTextColor(TFT_WHITE);
      spr.drawString("Send", 253, 5.8 * FONT_ROW_HEIGHT, 2);
      spr.drawString("Failed", 250, 6.6 * FONT_ROW_HEIGHT, 2);

      spr.setFreeFont(FSS9);
      spr.setTextColor(tft.color565(0, 139, 0), TFT_BLACK);             //Networking status indication：ON
      spr.drawString("LoRa(SenseCAP)", 82, 218 , GFXFF);
    }
    if(key == 3)  //Press the C button to disconnect
    {
      spr.fillRect(78, 110, 3, 11, tft.color565(140, 42, 42));          //No signal
      spr.fillRect(84, 107, 3, 14, tft.color565(140, 42, 42));   
      spr.fillRect(90, 104, 3, 17, tft.color565(140, 42, 42));
      spr.fillRect(96, 101, 3, 20, tft.color565(140, 42, 42));

      spr.fillCircle(265, 4.9 * FONT_ROW_HEIGHT, 10, TFT_RED);          //Data transmission status: join failed
      spr.setTextColor(TFT_WHITE);
      spr.drawString("Join LoRaWAN", 220, 5.8 * FONT_ROW_HEIGHT, 2);
      spr.drawString("Failed", 250, 6.6 * FONT_ROW_HEIGHT, 2);

      spr.setFreeFont(FSS9);
      spr.setTextColor(TFT_RED);                                        //Networking status indication：OFF
      spr.drawString("OFF", 82, 218 , GFXFF);
    }

    spr.setFreeFont(FSS9);
    spr.setTextColor(TFT_WHITE);
    spr.drawString("Disconnect", 29 * PIXEL, 8.4 * FONT_ROW_HEIGHT, GFXFF);
    
    spr.setFreeFont(FSS9);
    spr.drawString("Network :", 5, 218 , GFXFF);

    spr.pushSprite(0, 0);
    spr.deleteSprite();
}

void Disconnect_Tip()       // Pop-up prompt for sensor insertion
{
  spr.createSprite(240, 116);
  spr.fillScreen(tft.color565(211, 211, 211));

  spr.setFreeFont(FSS9);
  spr.setTextColor(TFT_BLACK);
  spr.drawString("Network will be disconnected", 3, 30, GFXFF);

  spr.fillRect(40, 70, 65, 35, tft.color565(140, 42, 42));
  spr.fillRect(139, 70, 65, 35, tft.color565(70, 130, 160));

  spr.setTextColor(TFT_WHITE);
  spr.drawString("Yes", 57, 78, GFXFF);
  spr.drawString("No", 162, 78, GFXFF);

  spr.pushSprite(40, 64);
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
  key();  //ABC button to select network
  ConnectSuc_Display(key_status);
  delay(2000);
  Disconnect_Tip();
  delay(2000);
}
