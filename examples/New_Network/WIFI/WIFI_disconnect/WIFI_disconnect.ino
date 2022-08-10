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

void wifi_head()
{
    spr.createSprite(120, 24);
    spr.fillSprite(tft.color565(100, 100, 100));
    spr.setFreeFont(FSS9);
    spr.setTextColor(TFT_WHITE);
    spr.drawString("WiFi(Ubidots)", 6, 3, GFXFF);
    spr.pushSprite(102, 50);
    spr.deleteSprite();
}

void wifi_info() ///
{
    spr.createSprite(200, 60);
    spr.fillSprite(TFT_BLACK);
    spr.setFreeFont(FSS9);
    spr.setTextColor(TFT_WHITE);
    spr.drawString("Connected:", 6, 6, 2);
    spr.drawString("WIFI(Ubidots)", 74, 6, 2);

    spr.drawString("SSID:", 6, 26, 2);
    spr.drawString("123456", 38, 26, 2);

    spr.drawString("Account:", 6, 46, 2);
    spr.drawString("user1", 60, 46, 2);

    spr.pushSprite(20, 76);
    spr.deleteSprite();
}

void disconnect_button()
{
    spr.createSprite(100, 34);
    spr.fillSprite(tft.color565(160, 34, 34));
    spr.setFreeFont(FSS9);
    spr.setTextColor(TFT_WHITE);
    spr.drawString("Disconnect", 5, 8, GFXFF);
    spr.pushSprite(115, 170);
    spr.deleteSprite();
}

void wifi_sig(int key)
{
    if (key == 2) // Press the A button to disconnect
    {
        spr.createSprite(80, 40);
        spr.setFreeFont(FSS9);
        spr.setTextColor(TFT_WHITE);
        spr.drawString("Signal:", 6, 15, 2);
        spr.fillRect(51, 16, 3, 11, tft.color565(0, 139, 0)); // No signal
        spr.fillRect(57, 13, 3, 14, tft.color565(0, 139, 0));
        spr.fillRect(63, 10, 3, 17, tft.color565(100, 100, 100));
        spr.fillRect(69, 7, 3, 20, tft.color565(100, 100, 100));
        spr.pushSprite(20, 140);
        spr.deleteSprite();
    }
    if (key == 3) // Press the A button to disconnect
    {
        spr.createSprite(80, 40);
        spr.setFreeFont(FSS9);
        spr.setTextColor(TFT_WHITE);
        spr.drawString("Signal:", 6, 15, 2);
        spr.fillRect(51, 16, 3, 11, tft.color565(0, 139, 0)); // No signal
        spr.fillRect(57, 13, 3, 14, tft.color565(0, 139, 0));
        spr.fillRect(63, 10, 3, 17, tft.color565(0, 139, 0));
        spr.fillRect(69, 7, 3, 20, tft.color565(100, 100, 100));
        spr.pushSprite(20, 140);
        spr.deleteSprite();
    }
    if (key == 4) // Press the A button to disconnect
    {

        spr.createSprite(80, 40);
        spr.setFreeFont(FSS9);
        spr.setTextColor(TFT_WHITE);
        spr.drawString("Signal:", 6, 15, 2);
        spr.fillRect(51, 16, 3, 11, tft.color565(0, 139, 0)); // No signal
        spr.fillRect(57, 13, 3, 14, tft.color565(0, 139, 0));
        spr.fillRect(63, 10, 3, 17, tft.color565(0, 139, 0));
        spr.fillRect(69, 7, 3, 20, tft.color565(0, 139, 0));
        spr.pushSprite(20, 140);
        spr.deleteSprite();
    }
    if (key == 1) // Press the B button to disconnect
    {
        spr.createSprite(80, 40);
        spr.setFreeFont(FSS9);
        spr.setTextColor(TFT_WHITE);
        spr.drawString("Signal:", 6, 15, 2);
        spr.fillRect(51, 16, 3, 11, tft.color565(0, 139, 0)); // No signal
        spr.fillRect(57, 13, 3, 14, tft.color565(100, 100, 100));
        spr.fillRect(63, 10, 3, 17, tft.color565(100, 100, 100));
        spr.fillRect(69, 7, 3, 20, tft.color565(100, 100, 100));
        spr.pushSprite(20, 140);
        spr.deleteSprite();

        spr.createSprite(100, 100);
        spr.fillCircle(28, 35, 10, TFT_YELLOW); // Data transmission status: join failed
        spr.setTextColor(TFT_YELLOW);
        spr.drawString("Low signal", 0, 60, 2);
        spr.drawString("intensity", 4, 85, 2);
        spr.pushSprite(240, 66);
        spr.deleteSprite();
    }
    if (key == 0) // Press the C button to disconnect
    {
        spr.createSprite(80, 50);
        spr.setFreeFont(FSS9);
        spr.setTextColor(TFT_WHITE);
        spr.drawString("Signal:", 6, 15, 2);
        spr.fillRect(51, 16, 3, 11, tft.color565(140, 42, 42)); // No signal
        spr.fillRect(57, 13, 3, 14, tft.color565(140, 42, 42));
        spr.fillRect(63, 10, 3, 17, tft.color565(140, 42, 42));
        spr.fillRect(69, 7, 3, 20, tft.color565(140, 42, 42));
        spr.pushSprite(20, 140);
        spr.deleteSprite();

        spr.createSprite(100, 100);
        spr.fillCircle(28, 35, 10, TFT_RED); // Data transmission status: join failed
        spr.setTextColor(TFT_RED);
        spr.drawString("Join WiFi", 0, 60, 2);
        spr.drawString("Failed", 10, 85, 2);
        spr.pushSprite(240, 66);
        spr.deleteSprite();
    }
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

int test_flag = 0;
int test_flag_1 = 0;
int joystick = 0;
void loop()
{
    if (digitalRead(WIO_KEY_A) == LOW)
    {
        test_flag_1++;
        delay(300);
        tft.fillScreen(TFT_BLACK);
    }

    Sense_Display(test_flag_1 % 3);

    if (digitalRead(WIO_KEY_B) == LOW)
    {
        test_flag++;
        delay(300);
        tft.fillScreen(TFT_BLACK);
    }

    Network_state(test_flag % 3); // 修改网络状态  左下角
    wifi_head();
    wifi_info();
    wifi_sig(test_flag % 5);
    disconnect_button();
    //    Disconnect_Tip(test_flag % 2); // 弹窗确认
    Below_Right_State_Content(test_flag % 5); // 修改 右下角 状态

    delay(1000);
}
