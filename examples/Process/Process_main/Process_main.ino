#include <TFT_eSPI.h>
#include "Free_Fonts.h"

TFT_eSPI tft;
TFT_eSprite spr = TFT_eSprite(&tft);

#define SCREEN_WIDTH 320                        // Wio Terminal Maximum Width
#define SCREEN_HIGH 240                         // Wio Terminal Maximum Height
#define PIXEL 4                                 // Width of one letter
#define MOVE_PIXEL_y 20                         // Virtual to Reality Moving Distance
const static unsigned int FONT_ROW_HEIGHT = 22; // The height of a letter
#include "RTC_SAMD51.h"
#include "DateTime.h"
#include <qrcode.h>

RTC_SAMD51 rtc;

DateTime now = DateTime(F(__DATE__), F(__TIME__));

void DISPLAY_INIT() // Display initialization, black background rotation
{
    tft.begin();
    tft.setRotation(3);
    tft.fillScreen(TFT_BLACK);
}

void Sense_Display(int CHOOSE_PAGE) // Sense interface display
{
    spr.createSprite(320, 70);
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

void Please_connect_to_Vision_AI_sensor(void)
{
    spr.createSprite(340,50);
    
    spr.setFreeFont(FSSB9);
    spr.setTextColor(TFT_WHITE);

    spr.drawString("Please connect to Vision AI Sensor", 9, 20, GFXFF);
    
    spr.pushSprite(0, 100);
    spr.deleteSprite();

    spr.createSprite(340,50);
    spr.pushSprite(0, 150);
    spr.deleteSprite();
}

int test_Vision_AI_real_time_data = 0;
void Process_main(int page)
{
    //此处加入 page 0;  page3 为了方便测试 可以调用此函数 也可以独立调用
    // case 0   Vision AI log
    // case 1  Proccsee首页
    // case 2  TinyML示例
    // case 3  TinyML已进入
            page = 0;
//        spr.createSprite(320, 70);

//        spr.fillRect(95, 50, 135, 20, tft.color565(118, 118, 118));
//        spr.drawString("TinyML Example", 95, 52, GFXFF);
//        spr.drawString("Visiom AI real-time analysis", 95, 52, GFXFF);

    switch (page)
    {
    case 0:
        spr.setFreeFont(FSSB9);
        Vision_AI_real_time_analysis(test_Vision_AI_real_time_data);
        test_Vision_AI_real_time_data++;
        return;
    case 1:
    // 270*80 = 21600
        spr.createSprite(270, 80);
        spr.setFreeFont(FSSB9);
        spr.setTextColor(TFT_WHITE);
        
        spr.setFreeFont(FSS9);
        spr.setTextColor(TFT_WHITE);

        spr.fillRect(22, 0, 75, 60, tft.color565(0, 139, 0));  // Green
        spr.drawString("Vision AI", 24, 0, GFXFF);
        spr.drawString("real-time", 24, 22, GFXFF);
        spr.drawString("analysis", 24, 44, GFXFF);
    
        spr.drawString("TinyML", 120, 0, GFXFF);
        spr.drawString("Example", 120, 22, GFXFF);

        spr.drawString("Data", 205, 0, GFXFF);
        spr.drawString("Filter (In ", 205, 20, GFXFF);
        spr.drawString("Develop", 205, 40, GFXFF);
        spr.drawString("ment)", 205, 60, GFXFF);
        
        spr.pushSprite(5, 100);
        
        spr.deleteSprite();
        break;
    case 2:
        spr.createSprite(270, 80);
        spr.setFreeFont(FSSB9);
        spr.setTextColor(TFT_WHITE);
        
        spr.setFreeFont(FSS9);
        spr.setTextColor(TFT_WHITE);
        
        spr.drawString("Vision AI", 24, 0, GFXFF);
        spr.drawString("real-time", 24, 22, GFXFF);
        spr.drawString("analysis", 24, 44, GFXFF);

        spr.fillRect(118, 0, 75, 40, tft.color565(0, 139,0));
        spr.drawString("TinyML", 120, 0, GFXFF);
        spr.drawString("Example", 120, 25, GFXFF);
    
        spr.drawString("Data", 205, 0, GFXFF);
        spr.drawString("Filter (In ", 205, 20, GFXFF);
        spr.drawString("Develop", 205, 40, GFXFF);
        spr.drawString("ment)", 205, 60, GFXFF);
        
        spr.pushSprite(5, 100);     
        spr.deleteSprite();
        break;
    case 3:
        Process_TinyML_ENTER();
        return;
    case 4:
        Please_connect_to_Vision_AI_sensor();
        return;
    default:;
    }
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
        spr.setTextColor(TFT_GREEN, TFT_BLACK);           // Networking status indication：ON
        spr.drawString("LoRa", 60, 0, 2); // Show the network you are in
        break;
    case 2:
        spr.setTextColor(TFT_GREEN, TFT_BLACK);          // Networking status indication：ON
        spr.drawString("WiFi", 60, 0, 2); // Show the network you are in
        break;
    default:;
    }
    spr.setTextColor(TFT_WHITE);
    spr.drawString("Network:", 5, 0, 2);
    spr.setFreeFont(FSS9);
    
    spr.pushSprite(0, 215);
    spr.deleteSprite();
}


void Process_TinyML_ENTER(void)
{
    spr.createSprite(130, 130);
    spr.setTextColor(TFT_WHITE);
//    spr.fillRect(95, 50, 135, 20, tft.color565(118, 118, 118));
//    spr.drawString("TinyML Example", 95, 52, GFXFF);
//    spr.drawString("Please scan the QR ", 135, 106, GFXFF);
//    spr.drawString("code on the screen ", 135, 126, GFXFF);
//    spr.drawString("to view the Github", 135, 146, GFXFF);
//    spr.drawString("sample tutorial ", 135, 166, GFXFF);
    double PIXELL = 3;

    spr.fillRect(15, 0, 113, 113, TFT_WHITE);
    QRCode qrcode;
    uint8_t *qrcodeData = (uint8_t *)malloc(qrcode_getBufferSize(5));
    qrcode_initText(&qrcode, qrcodeData, 5, 0, "https://wiki.seeedstudio.com/K1100-Getting-Started/#tinyml-section");
    for (uint8_t y = 0; y < qrcode.size; y++)
    {
        // Each horizontal module
        for (uint8_t x = 0; x < qrcode.size; x++)
        {
            if (qrcode_getModule(&qrcode, x, y))
                spr.fillRect(x * PIXELL + 15+1 , y * PIXELL+1 , PIXELL, PIXELL, TFT_BLACK);
        }
    }

    free(qrcodeData);
    spr.pushSprite(20, 80);
    spr.deleteSprite();

    spr.createSprite(130, 130);
    spr.setFreeFont(FSS9); 
    // code to view the tutorial
    spr.drawString("Scan the QR ", 0, 26, GFXFF);
    spr.drawString("code to view ", 0, 46, GFXFF);
    spr.drawString("the tutorial ", 0, 66, GFXFF);

    spr.pushSprite(160, 80);
    spr.deleteSprite();

}

int i = 0;
char buf_T[4][20];
int time_flag = 4;

void Vision_AI_real_time_analysis(int i_data) // todo
{
    spr.createSprite(230, 95);
    
    spr.setFreeFont(FSSB9);
    spr.setTextColor(TFT_WHITE);
    
    spr.fillRect(0, 0, 230, 94, tft.color565(128, 128, 128));
    DateTime now = rtc.now();

    uint8_t HH = now.hour();
    uint8_t MM = now.minute();
    uint8_t SS = now.second();

    int kk = i % 4;

    if (1)
    {
        for (int oo = 0; oo < 3; oo++)
        {
            for (int i = 0; i < 20; i++)
            {
                buf_T[oo][i] = buf_T[oo + 1][i];
            }
        }
    }

    else
    {
        ;
    }

    sprintf(buf_T[3], " %02d:%02d:%02d      %d", now.hour(), now.minute(), now.second(), i_data);

    for (int gg = 0; gg < 4; gg++)
    {
        spr.setFreeFont(FSSB9);
        spr.drawString(buf_T[gg], 45, 103 + gg * 23 - 100 , GFXFF);
    }

    for (int i = 0; i < time_flag - 1; i++)
    {
        spr.setFreeFont(FSSB9);
        spr.drawString(" 00:00:00      00", 45, 103 + i * 23 - 100, GFXFF);
    }

    if (time_flag < 0)
    {
        time_flag = 0;
    }
    else
    {
        time_flag--;
    }

    spr.pushSprite(40, 90);
    spr.deleteSprite();
    delay(1000);
}

void Show_title_in_Process(){
  spr.createSprite(320, 20);
  spr.drawString("Vision AI real-time analysis", 45, 52, GFXFF);
    spr.pushSprite(0, 180);
    spr.deleteSprite();
}

void Below_Right_State_Content(int gg_state)  // SD 插拔状态 Grove 插拔状态
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

void title(){
    ;
}

void setup()
{
    DISPLAY_INIT();
    rtc.begin();
    DateTime now = DateTime(F(__DATE__), F(__TIME__));
    Serial.println("adjust time!");
    rtc.adjust(now);

    pinMode(WIO_KEY_A, INPUT_PULLUP);
    pinMode(WIO_KEY_B, INPUT_PULLUP);
    pinMode(WIO_KEY_C, INPUT_PULLUP);
}

int gg_switch = 0;
int gg_switch_flag = 0;
int gg_network_flag = 0;
void loop()
{
    // test block begin
    if (digitalRead(WIO_KEY_B) == LOW)
    {
        Serial.println("B Key pressed");
        gg_switch++;
        delay(200);
    }
    if (digitalRead(WIO_KEY_C) == LOW)
    {
        Serial.println("C Key pressed");
        gg_network_flag++;
        delay(200);
    }

    Sense_Display(gg_switch % 3);

    Process_main(gg_switch % 5);
    
    Network_state(gg_switch % 3);
    Below_Right_State_Content(gg_switch % 5);
}
