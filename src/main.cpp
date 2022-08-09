#include <TFT_eSPI.h>
#include "Free_Fonts.h"

TFT_eSPI tft;
TFT_eSprite spr = TFT_eSprite(&tft);

#define SCREEN_WIDTH 320                        // Wio Terminal Maximum Width
#define SCREEN_HIGH 240                         // Wio Terminal Maximum Height
#define PIXEL 4                                 // Width of one letter
const static unsigned int FONT_ROW_HEIGHT = 22; // The height of a letter

void DISPLAY_INIT() // Display initialization, black background rotation
{
    pinMode(A0, INPUT);
    pinMode(WIO_LIGHT, INPUT);
    pinMode(WIO_KEY_A, INPUT_PULLUP);
    pinMode(WIO_KEY_B, INPUT_PULLUP);
    pinMode(WIO_KEY_C, INPUT_PULLUP);
    tft.begin();
    tft.setRotation(3);
    tft.fillScreen(TFT_BLACK);
}

// 320 * 25 = 8000
void Network_state(int s_key)
{
    spr.createSprite(140, 25);
    spr.setFreeFont(FSSB9);
    spr.fillSprite(TFT_BLACK);

    switch (s_key)
    {
    case 0:
        spr.setTextColor(TFT_RED);
        spr.drawString("OFF", 60, 0, 2);
        break;
    case 1:
        spr.setTextColor(TFT_GREEN, TFT_BLACK); // Networking status indication：ON
        spr.drawString("LoRa", 60, 0, 2);       // Show the network you are in
        break;
    case 2:
        spr.setTextColor(TFT_GREEN, TFT_BLACK); // Networking status indication：ON
        spr.drawString("WiFi", 60, 0, 2);       // Show the network you are in
        break;
    default:;
    }
    spr.setTextColor(TFT_WHITE);
    spr.drawString("Network:", 5, 0, 2);
    spr.setFreeFont(FSS9);

    spr.pushSprite(0, 215);
    spr.deleteSprite();
}

int GG;

// 320*70 = 22400
void Sense_Display(int CHOOSE_PAGE) // Sense interface display
{
    spr.createSprite(320, 50);
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

void Banner()
{
    spr.createSprite(148, 19);

    spr.setFreeFont(FSS9);
    spr.setTextColor(TFT_WHITE, tft.color565(112, 112, 112));
    spr.drawString("LoRa(SenseCAP)", 0, 0, GFXFF);

    spr.pushSprite(85, 50);
    spr.deleteSprite();
}

void ConnectSuc_Display(int key) // Select Frequency band interface
{
    spr.createSprite(SCREEN_WIDTH, SCREEN_HIGH);


    spr.setTextColor(TFT_WHITE);
    spr.drawString("Connected: LoRa (SenseCAP)", 30, 3.8 * FONT_ROW_HEIGHT, 2);
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

//      spr.fillCircle(265, 4.9 * FONT_ROW_HEIGHT, 10, TFT_GREEN);        //Data transmission status: normal
      spr.setTextColor(TFT_WHITE);
//      spr.drawString("Send", 253, 5.8 * FONT_ROW_HEIGHT, 2);
//      spr.drawString("Success", 245, 6.6 * FONT_ROW_HEIGHT, 2);

      spr.setFreeFont(FSS9);
      spr.setTextColor(tft.color565(0, 139, 0), TFT_BLACK);             //Networking status indication：ON
      spr.drawString("LoRa(SenseCAP)", 82, 218 , GFXFF);
    }
    if(key == 1)  //Press the A button to disconnect
    {
      spr.fillRect(78, 110, 3, 11, tft.color565(0, 139, 0));            //Four-frame signal
      spr.fillRect(84, 107, 3, 14, tft.color565(0, 139, 0));   
      spr.fillRect(90, 104, 3, 17, tft.color565(0, 139, 0));
      spr.fillRect(96, 101, 3, 20, tft.color565(0, 139, 0));

//      spr.fillCircle(265, 4.9 * FONT_ROW_HEIGHT, 10, TFT_GREEN);        //Data transmission status: normal
      spr.setTextColor(TFT_WHITE);
//      spr.drawString("Send", 253, 5.8 * FONT_ROW_HEIGHT, 2);
//      spr.drawString("Success", 245, 6.6 * FONT_ROW_HEIGHT, 2);

      spr.setFreeFont(FSS9);
      spr.setTextColor(tft.color565(0, 139, 0), TFT_BLACK);             //Networking status indication：ON
      spr.drawString("LoRa(SenseCAP)", 82, 218 , GFXFF);
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

void Disconnect_Tip(int _YES_NO_SELECTION) // Pop-up prompt for sensor insertion 0 NO 1 Yes
{
    spr.createSprite(200, 90);
    spr.fillScreen(tft.color565(211, 211, 211));

    spr.setFreeFont(FSS9);
    spr.setTextColor(TFT_BLACK);
    spr.drawString("Disconnected?", 38, 8, GFXFF);

    //  _YES_NO_SELECTION = 1;
    switch (_YES_NO_SELECTION)
    {
    case 0:
        spr.fillRect(14, 40, 73, 43, tft.color565(0, 250, 2));
        break;
    case 1:
        spr.fillRect(113, 40, 73, 43, tft.color565(0, 250, 2));
        break;
    default:;
    }

    spr.fillRect(18, 44, 65, 35, tft.color565(140, 42, 42));
    spr.fillRect(117, 44, 65, 35, tft.color565(70, 130, 160));

    spr.setTextColor(TFT_WHITE);
    spr.drawString("Yes", 35, 54, GFXFF);
    spr.drawString("No", 139, 54, GFXFF);

    spr.pushSprite(60, 86);
    spr.deleteSprite();
}

void setup()
{
    DISPLAY_INIT();
}

// int test_flag = 0;
int test_flag_1 = 0;

void loop()
{

    // Sense_Display(2);
    // Network_state(1);
    // Banner();
    // Disconnect_Tip(test_flag_1 % 2);
    // Below_Right_State_Content(1);

    test_flag_1++;
    ConnectSuc_Display(test_flag_1 % 4 );
    delay(1000);
}
