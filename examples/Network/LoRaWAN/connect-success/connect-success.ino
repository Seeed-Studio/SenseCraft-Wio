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
    tft.fillScreen(TFT_WHITE);
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

void Lora_Banner()  //Lora title
{
    spr.createSprite(148, 19);

    spr.setFreeFont(FSS9);
    spr.setTextColor(TFT_WHITE, tft.color565(112, 112, 112));
    spr.drawString("LoRa(SenseCAP)", 3, 0, GFXFF);

    spr.pushSprite(85, 50);
    spr.deleteSprite();
}

void ConnectSuc_Display(int STATE) // conn
{
    spr.createSprite(188, 95);

    spr.setTextColor(TFT_WHITE);
    spr.drawString("Connected: LoRa (SenseCAP)", 5, 3.8 * FONT_ROW_HEIGHT - 80, 2);
    spr.drawString("Signal:", 5, 4.8 * FONT_ROW_HEIGHT - 75, 2);
    spr.drawString("All data:", 5, 5.8 * FONT_ROW_HEIGHT - 75, 2);
    spr.drawString("packets", 115, 5.8 * FONT_ROW_HEIGHT - 75, 2);
    spr.drawString("Success:", 5, 6.8 * FONT_ROW_HEIGHT - 75, 2);
    spr.drawString("packets", 115, 6.8 * FONT_ROW_HEIGHT - 75, 2);

    spr.setFreeFont(FSSB9);
    spr.setTextColor(tft.color565(0, 139, 0));
    spr.drawString("10000", 65, 5.8 * FONT_ROW_HEIGHT - 75, 2); // Show total number of packages issued
    spr.drawString("999", 65, 6.8 * FONT_ROW_HEIGHT - 75, 2);   // Shows the number of successful deliveries

    switch (STATE)
    {
    case 0:
        spr.fillRect(53, 110 - 75, 3, 11, tft.color565(140, 42, 42)); // No signal
        spr.fillRect(59, 107 - 75, 3, 14, tft.color565(140, 42, 42));
        spr.fillRect(65, 104 - 75, 3, 17, tft.color565(140, 42, 42));
        spr.fillRect(71, 101 - 75, 3, 20, tft.color565(140, 42, 42));
        break;
    case 1:
        spr.fillRect(53, 110 - 75, 3, 11, tft.color565(0, 139, 0)); // One frame signal
        spr.fillRect(59, 107 - 75, 3, 14, tft.color565(100, 100, 100));
        spr.fillRect(65, 104 - 75, 3, 17, tft.color565(100, 100, 100));
        spr.fillRect(71, 101 - 75, 3, 20, tft.color565(100, 100, 100));
        break;
    case 2:
        spr.fillRect(53, 110 - 75, 3, 11, tft.color565(0, 139, 0)); // Two-frame signal
        spr.fillRect(59, 107 - 75, 3, 14, tft.color565(0, 139, 0));
        spr.fillRect(65, 104 - 75, 3, 17, tft.color565(100, 100, 100));
        spr.fillRect(71, 101 - 75, 3, 20, tft.color565(100, 100, 100));
        break;
    case 3:
        spr.fillRect(53, 110 - 75, 3, 11, tft.color565(0, 139, 0)); // Three-frame signal
        spr.fillRect(59, 107 - 75, 3, 14, tft.color565(0, 139, 0));
        spr.fillRect(65, 104 - 75, 3, 17, tft.color565(0, 139, 0));
        spr.fillRect(71, 101 - 75, 3, 20, tft.color565(100, 100, 100));
        break;
    case 4:
        spr.fillRect(53, 110 - 75, 3, 11, tft.color565(0, 139, 0)); // Four-frame signal
        spr.fillRect(59, 107 - 75, 3, 14, tft.color565(0, 139, 0));
        spr.fillRect(65, 104 - 75, 3, 17, tft.color565(0, 139, 0));
        spr.fillRect(71, 101 - 75, 3, 20, tft.color565(0, 139, 0));
        break;

    default:;
    }
    spr.pushSprite(20, 80);
    spr.deleteSprite();
}

void Lora_Connection_State(int STATE)
{
    spr.createSprite(90, 75);
    switch (STATE)
    {
    case 0:                                                                 
        spr.fillCircle(265 - 220, 4.9 * FONT_ROW_HEIGHT - 90, 10, tft.color565(160, 34, 34)); // Data transmission status: join failed
        spr.setTextColor(TFT_WHITE);
        spr.drawString("Join LoRaWAN", 220 - 220, 5.8 * FONT_ROW_HEIGHT - 90, 2);
        spr.drawString("Failed", 250 - 220, 6.6 * FONT_ROW_HEIGHT - 90, 2);
        break;
    case 1:
        spr.fillCircle(265 - 220, 4.9 * FONT_ROW_HEIGHT - 90, 10, tft.color565(255, 165, 0)); // Data transmission status: Packet loss
        spr.setTextColor(TFT_WHITE);
        spr.drawString("Send", 253 - 220, 5.8 * FONT_ROW_HEIGHT - 90, 2);
        spr.drawString("Failed", 250 - 220, 6.6 * FONT_ROW_HEIGHT - 90, 2);
        break;
    default:;
    }
    spr.pushSprite(208, 80);
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

void Disconnect_button()
{

    spr.createSprite(100, 30);
    spr.fillRect( 0, 0, 100, 30,  tft.color565(160, 34, 34));
    spr.setFreeFont(FSS9);
    spr.setTextColor(TFT_WHITE);
    spr.drawString("Disconnect", 4, 6, GFXFF);
    spr.pushSprite(105, 180);
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

    Sense_Display(2);
    Network_state(1);
    Banner();
    // Disconnect_Tip(test_flag_1 % 2);
    // Below_Right_State_Content(1);
    Lora_Connection_State(test_flag_1 % 2);
    test_flag_1++;
    ConnectSuc_Display(test_flag_1 % 4);
    Disconnect_button();
    delay(1000);
}
