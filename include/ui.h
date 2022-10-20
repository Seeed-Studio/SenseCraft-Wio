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

#define SENSOR_NUM_MAX 5      // Maximum number of sensors
#define LINE_DATA_MAX_SIZE 30 // maximum size of data
#define DRAW_LINE_MAX_NUM 3   // maximum num of draw line
#define SHOW_LOG_MAX_SIZE 11  // maximum size of log

#define WIDTH 32
#define X_OFFSET 16
#define X_RADIUS 18
#define HEIGHT 20
#define Y_OFFSET 10
#define Y_RADIUS 12

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

    uint8_t rotate_status = 0;
    bool    rotate_flag   = false;

    uint8_t            buff[256];
    struct sensor_data sdata;

    struct PagesStateMachine page;

    bool data_refresh;   // if new data push, refresh data in screen
    bool layout_refresh; // if a button pressed, refresh layout
    
    void render_frame(void);
    void build_frame(void);

    /* Label: basic and simple UI elements */ 
    void Label_Network(void);
    void Label_Hardware(uint8_t status);
    void Label_SensorInfo(String name, String unit, uint8_t pos);
    void Label_SensorData(sensor_data& data, uint8_t pos, uint16_t bg_color);
    void Label_SensorAdd(uint8_t pos);
    void Label_Subtitle(String value);
    void Label_CentreBtn(String name, uint16_t color);

    /* Widget: advanced and complex UI elements */
    void Widget_Title(uint8_t t);
    void Widget_SaveButton(uint8_t button);
    void Widget_Signal(int16_t signal, int32_t x, int32_t y);
    void Widget_PagePos(int PAGES, int _CHOOSE_PAGE);
    void Widget_LoraState(int32_t x, int32_t y);

    void StatusMachine(struct State *ui_state, uint8_t key);

    struct LoRaBandInfo lora_band_info[3] = {
        {"US", "915", "North America", 2},
        {"EU", "868", "European Region", 1},
        {"AU", "915", "Australia", 9},
    };

    struct NetworkState n_state = {0, {0, false, 0}, {0, false, 0}};
    void                NetworkPageManager(uint8_t keys);
    bool                Network_1(uint8_t select);
    bool                Network_2_0(uint8_t select); // lora
    bool                Network_2_1(uint8_t select); // wifi
    bool                Network_3_0(uint8_t select); // lora
    bool                Network_Disconnect(uint8_t select); // wifi
    bool                Network_4_0(uint8_t select); // lora
    bool                Network_4_1(uint8_t select); // wifi
    bool                Network_6_0(uint8_t select); // lora
    void NetworkLoRaBandSelect(uint8_t pos, struct LoRaBandInfo lbi, uint8_t select);

    struct State p_state = {0, true, 0};
    void         ProcessPageManager(uint8_t keys);
    bool         Process_1(uint8_t select);
    bool         Process_2(uint8_t select);

    struct State s_state = {0, true, 0};
    void         SensePageManager(uint8_t keys);
    bool         Sensor_1(uint8_t select);
    bool         Sensor_2(uint8_t select);
    bool         Sensor_3(uint8_t select);

    typedef bool (UI::*page_t)(uint8_t key);

    page_t l_network[6] = {&UI::Network_1, &UI::Network_2_0, &UI::Network_3_0, &UI::Network_4_0,
                           &UI::Network_Disconnect, &UI::Network_6_0};
    page_t w_network[4] = {&UI::Network_1, &UI::Network_2_1, &UI::Network_Disconnect, &UI::Network_4_1};

    page_t process[2] = {&UI::Process_1, &UI::Process_2};
    page_t sense[3]   = {&UI::Sensor_1, &UI::Sensor_2, &UI::Sensor_3};

    void PageMangent(uint8_t key);
    void (UI::*get_page[3])(uint8_t key) = {&UI::NetworkPageManager, &UI::ProcessPageManager, &UI::SensePageManager};

  private:
    // inline function, 4byte uint8_t to float
    void uint8_to_float(uint8_t *data, float *destination);

    // temp data
    int     temp_light;
    int     temp_mic;
    doubles line_chart_data[4];
};

#endif // __UI_H__
