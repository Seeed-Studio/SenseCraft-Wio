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

enum page_state{
    NETWORKPAGE,
    PROCESSPAGE,
    SENSEPAGE
};

#define NONE_PRESSED 0x0F

//define a struct with keyboard statemachine
struct PagesStateMachine
{
    page_state mainstate;
    uint8_t key;
    int8_t sense_select;
    int8_t  process_select;
    int8_t  network_select;
};
struct SenseState{
    int8_t current_page;
    int8_t sense_window;
    int8_t sense_select;
};

typedef bool (*page_t)(uint8_t key);

class UI : public Thread
{
public:
    UI(TFT_eSPI &lcd, TFT_eSprite &display, SysConfig &config, Message &m1);
    void init();

protected:
    virtual void Run();

    
    // void sense_2();
    // void sense_3();

public:
    void UIPushData(std::vector<sensor_data *> d);

private:
    TFT_eSPI &tft;
    TFT_eSprite &spr;

    Message &btnMail;

    SysConfig &cfg;

    std::vector<sensor_data> s_data;
    bool                     s_data_ready = true;



    uint8_t buff[256];
    struct  sensor_data sdata;


    struct PagesStateMachine page;

    void TitleDisplay(uint8_t t);
    void Status1Display(uint8_t status);
    void Status2Display(uint8_t status);
    bool Network_1(uint8_t keys);
    bool NetworkSubtitles(uint8_t keys);

    bool Process_1(uint8_t keys);
    void ProcessSubTitle(uint8_t t);

    struct SenseState s_sense = {0,0};

    void SensePageManager(uint8_t keys);
    bool Sensor_1(uint8_t keys);
    void SensorADDDisplay(uint8_t chose);
    void SensorPageState(int PAGES, int _CHOOSE_PAGE);
    void SensorSubTitle(String value);


    typedef bool (UI::*page_t)(uint8_t key);



    page_t network[3 ] = {&UI::Network_1};
    page_t process[3 ] = {&UI::Process_1};
    page_t sense[1]= {&UI::Sensor_1};

    void PageMangent(uint8_t key);

private:
    //inline function, 4byte uint8_t to float
    void uint8_to_float(uint8_t *data, float* destination);

    //temp data
    int temp_light;
    int temp_mic;
    doubles line_chart_data;
};

#endif // __UI_H__
