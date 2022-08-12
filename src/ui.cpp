#include "ui.h"

// inline function, 4byte uint8_t to float
void UI::uint8_to_float(uint8_t *data, float *destination) {
    uint32_t value =
        data[0] | ((uint32_t)data[1]) << 8 | ((uint32_t)data[2]) << 16 | ((uint32_t)data[1]) << 24;
    *reinterpret_cast<uint32_t *>(destination) = value;
}

UI::UI(TFT_eSPI &lcd, TFT_eSprite &display, SysConfig &config, Message &m1)
    : Thread("UIThread", 1024, 1), tft(lcd), spr(display), cfg(config), btnMail(m1) {
    Start();
};

void UI::init() {
}

void UI::Run() {
    uint8_t nums;
    uint8_t keys   = 2;
    page.mainstate = SENSEPAGE;
    page.key       = 0;
    page.process_select    = 0;
    page.sense_select      = 0;
    page.network_select    = 0;
    init();

    while (true) {
        nums = btnMail.Receive(&buff, 256, 0);
        if (nums > 0) {
            LOGSS.printf("btn Receive: %d ", nums);
            LOGSS.println(buff[0]);
            keys = buff[0] - 1;
        }

        // nums = sensorMail.Receive(&sdata, 256, 0);
        // if (nums > 0) {
            // for (size_t i = 0; i < sdata.size; i = i + 4) {
            //     LOGSS.printf("%d ", ((int32_t *)sdata.data)[i]);
            // }
            // temp_light = sdata.size;
            // temp_mic   = ((int32_t *)sdata.data)[sdata.size - 5];
            // LOGSS.printf("sensor Receive: %s %d %d\r\n", sdata.name, sdata.id, sdata.size);
            // LOGSS.printf("UI thread free memory: %d\r", xPortGetFreeHeapSize());
        //}
        LOGSS.printf("UI Stacks Free Bytes Remaining %d\r\n",
                     uxTaskGetStackHighWaterMark(GetHandle()));
        PageMangent(keys);
        Delay(Ticks::MsToTicks(1000));
    }
}

#define PIXEL 4 // Width of one letter
#define LEFT_SIDE 70
#define HIGHT_SIDE 47
const static unsigned int FONT_ROW_HEIGHT = 22; // The height of a letter

void UI::PageMangent(uint8_t key) {
    if (key < 3 && key != page.mainstate) {
        tft.fillScreen(TFT_BLACK);
        page.mainstate = (page_state)key;
        page.key = NONE;
    }else
        page.key = key;

    switch (page.mainstate) {
    case NETWORKPAGE:
        (this->*network[0])(0);
        break;
    case PROCESSPAGE:
        (this->*process[0])(page.key);
        break;
    case SENSEPAGE:
        SensePageManager(page.key);
        break;
    }
}
void UI::TitleDisplay(uint8_t t) {
    spr.createSprite(320, 46);
    spr.setFreeFont(FSSB9);
    switch (t) {
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

void UI::Status1Display(uint8_t status) {
    spr.createSprite(140, 25);
    spr.setFreeFont(FSSB9);
    spr.fillSprite(TFT_BLACK);

    switch (status) {
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

void UI::Status2Display(uint8_t status) {
    spr.createSprite(320, 25);
    spr.setFreeFont(FSSB9);
    spr.fillSprite(TFT_BLACK);
    // int s_key = 1;
    switch (status) {
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
    spr.deleteSprite();
}
bool UI::NetworkSubtitles(uint8_t keys) {
    switch (keys) {
    // LoRa
    case 0:
        spr.createSprite(155, 30);
        spr.fillSprite(tft.color565(100, 100, 100));
        spr.setFreeFont(FSS9);
        spr.setTextColor(TFT_WHITE);
        spr.drawString("LoRa(SenseCAP)", 6, 6, GFXFF);
        spr.pushSprite(85, 50);
        spr.deleteSprite();
        break;
    // WiFi
    case 1:
        spr.createSprite(120, 30);
        spr.fillSprite(tft.color565(100, 100, 100));
        spr.setFreeFont(FSS9);
        spr.setTextColor(TFT_WHITE);
        spr.drawString("WiFi(Ubidots)", 6, 6, GFXFF);
        spr.pushSprite(102, 50);
        spr.deleteSprite();
        break;
    }
}

bool UI::Network_1(uint8_t keys) {
    TitleDisplay(2);
    NetworkSubtitles(keys);

    switch (keys) {
        // LoRa
    case 0:
        spr.createSprite(110, 60);
        spr.fillSprite(tft.color565(0, 139, 0));
        spr.setFreeFont(FSS9);
        spr.setTextColor(TFT_WHITE);
        spr.drawString("LoRa", 6, 6, GFXFF);
        spr.drawString("(SenseCAP)", 5, 36, GFXFF);
        spr.pushSprite(30, 95);
        spr.deleteSprite();

        spr.createSprite(110, 60);
        spr.fillSprite(TFT_BLACK);
        spr.setFreeFont(FSS9);
        spr.setTextColor(TFT_WHITE);
        spr.drawString("WIFI", 6, 6, GFXFF);
        spr.drawString("(Ubidots)", 6, 36, GFXFF);
        spr.pushSprite(200, 95);
        spr.deleteSprite();

        spr.createSprite(240, 50);
        spr.fillSprite(TFT_BLACK);
        spr.setFreeFont(FSS9);
        spr.setTextColor(TFT_WHITE);
        spr.drawString("Please press the bottom right button", 6, 6, 2);
        spr.drawString("to confirm your network selection.", 6, 26, 2);
        spr.pushSprite(35, 160);
        spr.deleteSprite();
        break;
        // WiFi
    case 1:
        spr.createSprite(110, 60);
        spr.fillSprite(TFT_BLACK);
        spr.setFreeFont(FSS9);
        spr.setTextColor(TFT_WHITE);
        spr.drawString("LoRa", 6, 6, GFXFF);
        spr.drawString("(SenseCAP)", 5, 36, GFXFF);
        spr.pushSprite(30, 95);
        spr.deleteSprite();

        spr.createSprite(110, 60);
        spr.fillSprite(tft.color565(0, 139, 0));
        spr.setFreeFont(FSS9);
        spr.setTextColor(TFT_WHITE);
        spr.drawString("WIFI", 6, 6, GFXFF);
        spr.drawString("(Ubidots)", 6, 36, GFXFF);
        spr.pushSprite(200, 95);
        spr.deleteSprite();

        spr.createSprite(300, 50);
        spr.fillSprite(TFT_BLACK);
        spr.setFreeFont(FSS9);
        spr.setTextColor(TFT_WHITE);
        spr.drawString("Please refer to the wikito modify the", 6, 6, 2);
        spr.drawString("configuration file and send it to this device.", 6, 26, 2);
        spr.pushSprite(15, 160);
        spr.deleteSprite();
        break;
    }

    // toDo: Network status
    Status1Display(0);
    return false;
}

void UI::ProcessSubTitle(uint8_t t) {
    switch (t) {
    case 0:
        spr.createSprite(320, 18);
        spr.setTextColor(TFT_WHITE);
        spr.fillRect(50, 0, 234, 94, tft.color565(100, 100, 100));
        spr.drawString("Vision AI real-time analysis", 50, 0, GFXFF);
        spr.pushSprite(0, 50);
        spr.deleteSprite();
        break;

    case 1:
        spr.createSprite(320, 18);
        spr.setTextColor(TFT_WHITE);
        spr.fillRect(90, 0, 145, 94, tft.color565(100, 100, 100));
        spr.drawString("TinyML Example", 90, 0, GFXFF);
        spr.pushSprite(0, 50);
        spr.deleteSprite();
        break;
    }
}

bool UI::Process_1(uint8_t key) {
    TitleDisplay(1);
    if (key == LEFT_PRESSED) {
        page.process_select--;
        if (page.process_select < 0) {
            page.process_select = 0;
        }
    } else if (key == RIGHT_PRESSED) {
        page.process_select++;
        if (page.process_select > 1) {
            page.process_select = 1;
        }
    }

    ProcessSubTitle(page.process_select);

    switch (page.process_select) {
    // Vision AI real-time analysis
    case 0:
        // 270*80 = 21600
        spr.createSprite(270, 80);
        spr.setFreeFont(FSSB9);
        spr.setTextColor(TFT_WHITE);

        spr.setFreeFont(FSS9);
        spr.setTextColor(TFT_WHITE);

        spr.fillRect(22, 0, 75, 60, tft.color565(0, 139, 0)); // Green
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
    // TinyML Example
    case 1:
        spr.createSprite(270, 80);
        spr.setFreeFont(FSSB9);
        spr.setTextColor(TFT_WHITE);

        spr.setFreeFont(FSS9);
        spr.setTextColor(TFT_WHITE);

        spr.drawString("Vision AI", 24, 0, GFXFF);
        spr.drawString("real-time", 24, 22, GFXFF);
        spr.drawString("analysis", 24, 44, GFXFF);

        spr.fillRect(118, 0, 75, 40, tft.color565(0, 139, 0));
        spr.drawString("TinyML", 120, 0, GFXFF);
        spr.drawString("Example", 120, 25, GFXFF);

        spr.drawString("Data", 205, 0, GFXFF);
        spr.drawString("Filter (In ", 205, 20, GFXFF);
        spr.drawString("Develop", 205, 40, GFXFF);
        spr.drawString("ment)", 205, 60, GFXFF);

        spr.pushSprite(5, 100);
        spr.deleteSprite();
        break;

    // toDo: Data Filter
    case 2:
        break;
    }
    // toDo: Network status
    Status1Display(0);
    return false;
}

void UI::SensePageManager(uint8_t key) {
    (this->*sense[0])(0);
}

void UI::SensorSubTitle(String value)
{
    spr.createSprite(320, 15);
    spr.setFreeFont(FSSB9);
    spr.setTextColor(TFT_WHITE, tft.color565(100, 100, 100));
    spr.drawString(value, 120, 0, GFXFF);
    spr.pushSprite(0, 50);
    spr.deleteSprite();
}

void UI::SensorADDDisplay(uint8_t is_backgroud){
    spr.createSprite(90, 100);

    if(is_backgroud){
        spr.fillRect(0, 0, 90, 100, tft.color565(0, 139, 0));   
    }
   
    spr.setFreeFont(FSS9);
    spr.setTextColor(TFT_WHITE);
    spr.drawString("ADD", 14, 5 ,4);

    spr.fillRect(20, 1.5 * FONT_ROW_HEIGHT, 40, 40, TFT_WHITE);
    spr.fillRect(38, 1.5 * FONT_ROW_HEIGHT + 6, 3, 26, TFT_BLACK);
    spr.fillRect(27, 1.5 * FONT_ROW_HEIGHT + 18, 26, 3, TFT_BLACK);

    spr.pushSprite(220, 90);
    spr.deleteSprite();    
}

void UI::SensorPageState(int pages_num, int page_select)
{

    spr.createSprite(340, 10);
                            // 问题
    int *page_location = new int[pages_num];

    int temp = 0;

    for (int i = 0; i < pages_num; i++)
    {
        page_location[temp] = 140 - pages_num / 2 * 10 * (1 + pages_num % 2) + i * 20;
        temp++;
    }

    for (int i = 0; i < pages_num; i++)
    {
        if (i == page_select)
            spr.fillCircle(page_location[i], 6, 3, tft.color565(0, 193, 255));
        else
            spr.fillCircle(page_location[i] , 6, 3, tft.color565(220, 220, 220));
    }
    delete page_location;
    spr.pushSprite(0, 200);
    spr.deleteSprite();
}

bool UI::Sensor_1(uint8_t) {

    struct _data_base
    {
        String value;
        String type;
    };

    struct _STRUCT_INPUT_DATA
    {
        _data_base data[4];
    } GG;

    GG.data[0].value = "A";
    GG.data[0].type = "'C";

    GG.data[1].value = "AA";
    GG.data[1].type = "kg";

    GG.data[2].value = "AAA";
    GG.data[2].type = "cd";

    GG.data[3].value = "AAAA";
    GG.data[3].type = "m/s";

    TitleDisplay(0);
    SensorSubTitle("Sensor Data");

    spr.createSprite(90, 100);
    //
    spr.fillRect(0, 0, 90, 100, tft.color565(0, 139, 0));
    spr.setFreeFont(FSS9);
    spr.setTextColor(TFT_WHITE);

    short int _SIZE;

    for (int i = 0; i < 4; i++)
    {   //last parameter is the size of the font
        spr.drawString(GG.data[i].value, 2, 5 + 24 * i, 4);
        spr.drawString(GG.data[i].type, 68, 5 + 24 * i, 2);
    }

    switch (0)
    {
    case 0:
        spr.pushSprite(20, 90);
        break;
    case 1:
        spr.pushSprite(120, 90);
        break;
    case 2:
        spr.pushSprite(220, 90);
        break;
    default:
        break;
    }

    spr.deleteSprite();

    SensorADDDisplay(1);
    SensorPageState(6,3);
    // TOdo

    // toDo: Network status
    Status1Display(0);
    return false;
}