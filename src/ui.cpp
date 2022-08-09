#include "ui.h"

// inline function, 4byte uint8_t to float
void UI::uint8_to_float(uint8_t *data, float *destination) {
    uint32_t value =
        data[0] | ((uint32_t)data[1]) << 8 | ((uint32_t)data[2]) << 16 | ((uint32_t)data[1]) << 24;
    *reinterpret_cast<uint32_t *>(destination) = value;
}

UI::UI(TFT_eSPI &lcd, TFT_eSprite &display, SysConfig &config, Message &m1, Message &m2)
    : Thread("UIThread", 1024, 1), tft(lcd), spr(display), cfg(config), btnMail(m1),
      sensorMail(m2) {
    Start();
};

void UI::init() {
}

void UI::Run() {
    uint8_t nums;

    init();

    while (true) {
        nums = btnMail.Receive(&buff, 256, 0);
        if (nums > 0) {
            Serial.printf("btn Receive: %d ", nums);
            Serial.println(buff[0]);
        }

        nums = sensorMail.Receive(&sdata, 256, 0);
        if (nums > 0) {
            // for (size_t i = 0; i < sdata.size; i = i + 4) {
            //     Serial.printf("%d ", ((int32_t *)sdata.data)[i]);
            // }
            // temp_light = sdata.size;
            // temp_mic   = ((int32_t *)sdata.data)[sdata.size - 5];
            // Serial.printf("sensor Receive: %s %d %d\r\n", sdata.name, sdata.id, sdata.size);
            // Serial.printf("UI thread free memory: %d\r", xPortGetFreeHeapSize());
        }

        for (int i = 0; i < 3; i++) {
            (this->*page[i])();
            Delay(Ticks::MsToTicks(100));
        }
    }
}

#define PIXEL 4 // Width of one letter
#define LEFT_SIDE 70
#define HIGHT_SIDE 47
const static unsigned int FONT_ROW_HEIGHT = 22; // The height of a letter

void UI::sense_1() {
    uint8_t random_num;
    spr.createSprite(320, 70);
    spr.setFreeFont(FSSB9);
    random_num = random(0, 3);
    switch (random_num)
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

void UI::sense_2() {
    uint8_t random_num;
    spr.createSprite(320, 25);
    spr.setFreeFont(FSSB9);
    spr.fillSprite(TFT_BLACK);
    random_num = random(0, 3);
    switch (random_num)
    {
    case 0:
        spr.setTextColor(TFT_RED);
        spr.drawString("OFF", 90, 0, GFXFF);
        break;
    case 1:
        spr.setTextColor(TFT_GREEN, TFT_BLACK);           // Networking status indication：ON
        spr.drawString("LoRa(SenseCAP)", 90, 0, GFXFF); // Show the network you are in
        break;
    case 2:
        spr.setTextColor(TFT_GREEN, TFT_BLACK);          // Networking status indication：ON
        spr.drawString("WIFI(Ubidots)", 90, 0, GFXFF); // Show the network you are in
        break;
    default:;
    }
    spr.setTextColor(TFT_WHITE);
    spr.drawString("Network :", 5, 0, GFXFF);
    spr.pushSprite(0, 215);
    spr.deleteSprite();
}


void UI::sense_3() {
    tft.fillRect(18, 78, 24, 90, TFT_WHITE);
    if (line_chart_data.size() > LINE_DATA_MAX_SIZE) // keep the old line chart front
    {
        line_chart_data.pop(); // this is used to remove the first read variable
    }
    line_chart_data.push(random(0, 255));


    // 85 * 260 = 22100
    auto content = line_chart(20, 80); //(x,y) where the line graph begins
    content
        .height(85)
        .width(260)
        .based_on(0.0)      // Starting point of y-axis, must be a float
        .show_circle(false) // drawing a cirle at each point, default is on.
        .value(line_chart_data)        // passing through the data to line graph
        .max_size(LINE_DATA_MAX_SIZE)
        .color(TFT_GREEN) // Setting the color for the line
                          //        .backgroud(tft.color565(0,0,0)) // Setting the color for the backgroud
        .backgroud(tft.color565(0, 0, 0))
        .draw(&tft);
}