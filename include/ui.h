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
#define DRAW_LINE_MAX_NUM 3   // maximum num of draw line
#define SHOW_LOG_MAX_SIZE 11  // maximum size of log

enum page_state { NETWORKPAGE, PROCESSPAGE, SENSEPAGE };

#define NONE_PRESSED 0x0F

#define FIRST_PAGE 0
#define LORA_PAGE 0
#define WIFI_PAGE 1

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

struct NetworkState {
    int8_t       current_network;
    struct State nw_state; // network wifi state
    struct State nl_state; // network lora state
};

struct LoRaBandInfo {
    char   *type;
    char   *frequency;
    char   *country;
    uint8_t band;
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
    void UIPushLog(std::vector<log_data> d);

  private:
    TFT_eSPI    &tft;
    TFT_eSprite &spr;

    Message &btnMail;

    SysConfig &cfg;

    std::vector<sensor_data> s_data;
    bool                     s_data_ready = true;

    std::vector<log_data> a_log;
    bool                  log_ready = true;

    uint8_t            buff[256];
    struct sensor_data sdata;

    struct PagesStateMachine page;

    void TitleDisplay(uint8_t t);
    void Status1Display(uint8_t status);
    void Status2Display(uint8_t status);

    void StatusMachine(struct State *ui_state, uint8_t key);

    struct LoRaBandInfo lora_band_info[3] = {
        {"US", "915", "North America", 2},
        {"EU", "868", "European Region", 1},
        {"AU", "915", "Australia", 9},
    };

    struct NetworkState n_state = {0, {0, false, 0}, {0, false, 0}};
    bool                NetworkPageManager(uint8_t keys);
    bool                Network_1(uint8_t select);
    bool                Network_2_0(uint8_t select); // lora
    bool                Network_2_1(uint8_t select); // wifi
    bool                Network_3_0(uint8_t select); // lora
    bool                Network_3_1(uint8_t select); // wifi
    bool                Network_4_0(uint8_t select); // lora
    bool                Network_5_0(uint8_t select); // lora
    bool                NetworkSubtitles(uint8_t keys);
    void NetworkLoRaBandSelect(uint8_t location, struct LoRaBandInfo lbi, uint8_t select);

    struct State p_state = {0, true, 0};
    void         ProcessPageManager(uint8_t keys);
    void         ProcessSubTitle(uint8_t select);
    bool         Process_1(uint8_t select);
    bool         Process_2(uint8_t select);

    struct State s_state = {0, true, 0};
    void         SensePageManager(uint8_t keys);
    bool         Sensor_1(uint8_t select);
    bool         Sensor_2(uint8_t select);
    void         SensorADDDisplay(uint8_t chose);
    void         SensorPageState(int PAGES, int _CHOOSE_PAGE);
    void         SensorSubTitle(String value);

    typedef bool (UI::*page_t)(uint8_t key);

    page_t l_network[5] = {&UI::Network_1, &UI::Network_2_0, &UI::Network_3_0, &UI::Network_4_0,
                           &UI::Network_5_0};
    page_t w_network[3] = {&UI::Network_1, &UI::Network_2_1, &UI::Network_3_1};

    page_t process[2] = {&UI::Process_1, &UI::Process_2};
    page_t sense[2]   = {&UI::Sensor_1, &UI::Sensor_2};

    void PageMangent(uint8_t key);

  private:
    // inline function, 4byte uint8_t to float
    void uint8_to_float(uint8_t *data, float *destination);

    // temp data
    int     temp_light;
    int     temp_mic;
    doubles line_chart_data[4];
};

#endif // __UI_H__
