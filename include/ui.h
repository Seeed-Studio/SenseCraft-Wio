#ifndef __UI_H__
#define __UI_H__
#include "Free_Fonts.h"
#include "Seeed_Arduino_ooFreeRTOS.h"
#include "SysConfig.h"
#include "seeed_line_chart.h"
#include "sensor.h"
#include "utils.h"
#include <Arduino.h>
#include <TFT_eSPI.h>
#include <stdint.h>

using namespace cpp_freertos;

#define SCREEN_WIDTH 320 // Wio Terminal Maximum Width
#define SCREEN_HIGH 240  // Wio Terminal Maximum Height

#define LINE_DATA_MAX_SIZE 30 // maximum size of data

enum page_state { NETWORKPAGE, PROCESSPAGE, SENSEPAGE };

#define NONE_PRESSED 0x0F

// define a struct with keyboard statemachine
struct PagesStateMachine {
    page_state mainstate;
    uint8_t    key;
    int8_t     sense_select;
    int8_t     process_select;
    int8_t     network_select;
};
struct State {
    int8_t current_page;
    bool   is_next;
    int8_t s_select;
};

typedef bool (*page_t)(uint8_t key);

class UI : public Thread {
  public:
    UI(TFT_eSPI &lcd, TFT_eSprite &display, SysConfig &config, Message &m1);
    void init();

  protected:
    virtual void Run();


  public:
    void UIPushData(std::vector<sensor_data *> d);

  private:
    TFT_eSPI    &tft;
    TFT_eSprite &spr;

    Message &btnMail;

    SysConfig &cfg;

    std::vector<sensor_data> s_data;
    bool                     s_data_ready = true;

    uint8_t            buff[256];
    struct sensor_data sdata;

    struct PagesStateMachine page;

    void TitleDisplay(uint8_t t);
    void Status1Display(uint8_t status);
    void Status2Display(uint8_t status);
    bool Network_1(uint8_t keys);
    bool NetworkSubtitles(uint8_t keys);

    struct State p_state = {0, true, 0};
    void ProcessPageManager(uint8_t keys);
    void ProcessSubTitle(uint8_t select);
    bool Process_1(uint8_t select);
    bool Process_2(uint8_t select);


    struct State s_state = {0, true, 0};
    void SensePageManager(uint8_t keys);
    bool Sensor_1(uint8_t select);
    bool Sensor_2(uint8_t select);
    void SensorADDDisplay(uint8_t chose);
    void SensorPageState(int PAGES, int _CHOOSE_PAGE);
    void SensorSubTitle(String value);

    typedef bool (UI::*page_t)(uint8_t key);

    page_t network[3] = {&UI::Network_1};
    page_t process[2] = {&UI::Process_1, &UI::Process_2};
    page_t sense[2]   = {&UI::Sensor_1, &UI::Sensor_2};

    void PageMangent(uint8_t key);

  private:
    // inline function, 4byte uint8_t to float
    void uint8_to_float(uint8_t *data, float *destination);

    // temp data
    int     temp_light;
    int     temp_mic;
    doubles line_chart_data;
};

#endif // __UI_H__
