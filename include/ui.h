#ifndef __UI_H__
#define __UI_H__
#include "Seeed_Arduino_ooFreeRTOS.h"
#include <Arduino.h>
#include "Free_Fonts.h"
#include <stdint.h>
#include <TFT_eSPI.h>
#include "sensor.h"
#include "utils.h"
#include "seeed_line_chart.h"
#include "SysConfig.h"

using namespace cpp_freertos;

#define SCREEN_WIDTH 320 // Wio Terminal Maximum Width
#define SCREEN_HIGH 240  // Wio Terminal Maximum Height

#define LINE_DATA_MAX_SIZE 30             // maximum size of data

class UI : public Thread
{
public:
    UI(TFT_eSPI &lcd, TFT_eSprite &display, SysConfig &config, Message &m1, Message &m2);
    void init();

protected:
    virtual void Run();

    void sense_1();
    void sense_2();
    void sense_3();

private:
    TFT_eSPI &tft;
    TFT_eSprite &spr;

    Message &btnMail;
    Message &sensorMail;

    SysConfig &cfg;



    uint8_t buff[256];
    struct  sensor_data sdata;


    void (UI::*page[3])() = {&UI::sense_1, &UI::sense_2, &UI::sense_3};
private:
    //inline function, 4byte uint8_t to float
    void uint8_to_float(uint8_t *data, float* destination);

    //temp data
    int temp_light;
    int temp_mic;
    doubles line_chart_data;
};

#endif // __UI_H__
