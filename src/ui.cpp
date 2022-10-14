#include "ui.h"
#include "qrcode.h"

#define countof(a) (sizeof(a) / sizeof(*(a)))

// inline function, 4byte uint8_t to float
void UI::uint8_to_float(uint8_t *data, float *destination) {
    uint32_t value =
        data[0] | ((uint32_t)data[1]) << 8 | ((uint32_t)data[2]) << 16 | ((uint32_t)data[1]) << 24;
    *reinterpret_cast<uint32_t *>(destination) = value;
}

extern void LogMemoryUsage(const char *s);
UI::UI(TFT_eSPI &lcd, TFT_eSprite &display, SysConfig &config, Message &m1)
    : Thread("UIThread", 128 * 6, 3), tft(lcd), spr(display), cfg(config), btnMail(m1) {
    Start();
};

void UI::init() {
    tft.fillRect(0, 45, 320, 195, TFT_BLACK);
}

void UI::Run() {
    uint8_t nums;
    uint8_t keys        = 2;
    page.mainstate      = SENSEPAGE;
    page.key            = 0;
    page.process_select = 0;
    page.sense_select   = 0;
    page.network_select = 0;
    s_data_ready        = true;
    init();

    while (true) {
        nums = btnMail.Receive(&buff, 256, 0);
        // Whether to rotate the screen
        if (rotate_status != rotate_flag) {
            rotate_status = rotate_flag;
            tft.fillScreen(TFT_BLACK);
            if (rotate_flag == true)
                tft.setRotation(1);
            else
                tft.setRotation(3);
        }
        if (nums > 0) {
            LOGSS.printf("btn Receive: %d ", nums);
            LOGSS.println(buff[0]);
            keys = buff[0] - 1;
        } else
            keys = NONE_PRESSED;

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
        // LOGSS.printf("UI Stacks Free Bytes Remaining %d\r\n",
        //              uxTaskGetStackHighWaterMark(GetHandle()));
        PageMangent(keys);
        //主要是等数据，不然会Sensor会get不到数据，无法切换页面，未来需要加锁
        Delay(Ticks::MsToTicks(50));
    }
}

// Store the received sensor data in the UI class
void UI::UIPushData(std::vector<sensor_data *> d) {
    // A loop to deep copy param of d vector into new lora_data queue
    // by Iterative method
    // ui刷新时不更新数据
    if (s_data_ready) {
        s_data.clear();
        s_data.shrink_to_fit();
        for (auto data : d) {
            s_data.push_back(*data);
            // Whether to rotate the screen
            if (data->name == "IMU") {
                if (((int32_t *)data->data)[0] > 50)
                    if (rotate_flag == true) {
                        rotate_flag = false;
                    }
                if (((int32_t *)data->data)[0] < -50)
                    if (rotate_flag == false) {
                        rotate_flag = true;
                    }
            }
        }
    }
}

void UI::UIPushLog(std::vector<log_data> d) {
    // ui刷新时不更新数据
    if (log_ready) {
        int log_num = d.size();
        // 最多只显示11条日志
        if (log_num > SHOW_LOG_MAX_SIZE)
            log_num = SHOW_LOG_MAX_SIZE;
        a_log.clear();
        a_log.shrink_to_fit();
        for (int i = 0; i < log_num; i++)
            a_log.push_back(d.at(d.size() - log_num + i));
    }
}

#define PIXEL 4 // Width of one letter
#define LEFT_SIDE 70
#define HIGHT_SIDE 47
const static unsigned int FONT_ROW_HEIGHT = 22; // The height of a letter

void UI::PageMangent(uint8_t key) {
    if (key < 3) {
        // tft.fillScreen(TFT_BLACK);
        init();
        page.mainstate = (page_state)key;
        key            = NONE_PRESSED;
    }
    switch (page.mainstate) {
    case NETWORKPAGE:
        NetworkPageManager(key);
        break;
    case PROCESSPAGE:
        ProcessPageManager(key);
        break;
    case SENSEPAGE:
        SensePageManager(key);
        break;
    }
}
void UI::TitleDisplay(uint8_t t) {
    static uint8_t cur = 3; 
    if(cur == t)
        return;
    else
        cur = t;
    for(uint8_t i = 0; i < 3; i++){
        uint32_t color = (i==t) ? (tft.color565(135, 206, 235)) : TFT_WHITE;
        tft.fillRoundRect((4+i*26)*PIXEL, 4, 21*PIXEL, FONT_ROW_HEIGHT+15, 8, color);
    }

    tft.setFreeFont(FSSB9);
    tft.setTextColor(TFT_BLACK);
    tft.drawString("Sense", 32, 14, GFXFF);
    tft.drawString("Process", 127, 14, GFXFF);
    tft.drawString("Uplink", 237, 14, GFXFF);
    tft.drawLine(0, 2*FONT_ROW_HEIGHT, SCREEN_WIDTH, 2*FONT_ROW_HEIGHT, TFT_WHITE);

}

void UI::Status1Display(uint8_t status) {
    tft.setFreeFont(FSSB9);
    if (cfg.wificonnected == true) {
        tft.setTextColor(TFT_GREEN, TFT_BLACK); // Networking status indication：ON
        tft.drawString("WiFi", 60, 215, 2);       // Show the network you are in
    } else if (cfg.lora_status == LORA_JOIN_SUCCESS || cfg.lora_status == LORA_SEND_SUCCESS ||
               cfg.lora_status == LORA_SEND_FAILED) {
        tft.setTextColor(TFT_GREEN, TFT_BLACK); // Networking status indication：ON
        tft.drawString("LoRa", 60, 215, 2);       // Show the network you are in
    } else {
        tft.setTextColor(TFT_RED, TFT_BLACK); // Networking status indication：OFF
        tft.drawString("OFF", 60, 215, 2);      // Show the network you are in
    }
    tft.setTextColor(TFT_WHITE);
    tft.drawString("Network:", 5, 215, 2);
}

void UI::Status2Display(uint8_t status) {
    tft.setFreeFont(FSS9);
    tft.setTextColor(TFT_YELLOW);
    switch (status) {
    case 0:
        tft.drawString("Plug in a Grove sensor", 170, 215, 2);
        break;
    case 1:
        tft.drawTriangle(140, 233, 151, 215, 162, 233, TFT_YELLOW);
        tft.drawString("! ", 150, 219, 2);
        tft.drawString("TF card storage is full", 168, 215, 2);
        break;
    case 2:
        tft.drawString("Vision AI Sensor connected", 142, 215, 2);
        break;
    case 3:
        tft.drawString("Saving has been started", 162, 215, 2);
        break;
    case 4:
        tft.drawString("Please insert TF card", 170, 215, 2);
        break;
    case 5:
        tft.drawString("No data needs to be stored", 146, 215, 2);
        break;
    default:
        tft.drawString("                             ", 170, 215, 2);
        break;
    }
}
bool UI::NetworkSubtitles(uint8_t keys) {
    switch (keys) {
    // LoRa
    case 0:
        spr.createSprite(85, 30);
        spr.fillSprite(tft.color565(100, 100, 100));
        spr.setFreeFont(FSS9);
        spr.setTextColor(TFT_WHITE);
        spr.drawString("   LoRa    ", 6, 6, GFXFF);
        spr.pushSprite(120, 50);

        spr.deleteSprite();
        break;
    // WiFi
    case 1:
        spr.createSprite(85, 30);
        spr.fillSprite(tft.color565(100, 100, 100)); // fill color below text
        spr.setFreeFont(FSS9);
        spr.setTextColor(TFT_WHITE);
        spr.drawString("   WiFi    ", 6, 6, GFXFF);
        spr.pushSprite(120, 50);
        spr.deleteSprite();
        break;
    }
}

// todo: 未启用，待完善
void UI::StatusMachine(struct State *ui_state, uint8_t key) {
    switch (key) {
    case LEFT_PRESSED:
        ui_state->s_select--;
        if (ui_state->s_select < 0) {
            ui_state->s_select = 0;
        }
        tft.fillScreen(TFT_BLACK);
        break;
    case RIGHT_PRESSED:
        ui_state->s_select++;
        // 只有两个选择
        if (ui_state->s_select > 2) {
            ui_state->s_select = 2;
        }
        tft.fillScreen(TFT_BLACK);
        break;
    case UP_PRESSED:
        ui_state->current_page--;
        if (ui_state->current_page < 0) {
            ui_state->current_page = 0;
        }
        tft.fillScreen(TFT_BLACK);
        break;
    case SELECT_PRESSED:
        if (ui_state->is_next) {
            ui_state->current_page++;
            if (ui_state->current_page > countof(process) - 1) {
                ui_state->current_page = countof(process) - 1;
            }
        }
        tft.fillScreen(TFT_BLACK);
        break;
    }
}

bool UI::NetworkPageManager(uint8_t keys) {
    switch (keys) {
    case LEFT_PRESSED:
        if (n_state.nl_state.current_page == FIRST_PAGE &&
            n_state.nw_state.current_page == FIRST_PAGE) {
            n_state.current_network = LORA_PAGE;
        }

        if (n_state.current_network == LORA_PAGE) {
            n_state.nl_state.s_select--;
            if (n_state.nl_state.s_select < 0) {
                n_state.nl_state.s_select = 0;
            }
            if (n_state.nl_state.current_page < countof(l_network) - 2) {
                // tft.fillScreen(TFT_BLACK);
                init();
            }
        } else { // WiFi
            n_state.nw_state.s_select--;
            if (n_state.nw_state.s_select < 0) {
                n_state.nw_state.s_select = 0;
            }
            if (n_state.nw_state.current_page != countof(w_network) - 2) {
                tft.fillScreen(TFT_BLACK);
            }
        }
        break;
    case RIGHT_PRESSED:
        if (n_state.nl_state.current_page == FIRST_PAGE &&
            n_state.nw_state.current_page == FIRST_PAGE) {
            n_state.current_network = WIFI_PAGE;
        }

        if (n_state.current_network == LORA_PAGE) {
            n_state.nl_state.s_select++;
            if (n_state.nl_state.s_select >
                sizeof(lora_band_info) / sizeof(lora_band_info[0]) - 1) {
                n_state.nl_state.s_select = sizeof(lora_band_info) / sizeof(lora_band_info[0]) - 1;
            }
            if (n_state.nl_state.current_page < countof(l_network) - 2) {
                // tft.fillScreen(TFT_BLACK);
                init();
            }
        } else { // WiFi
            n_state.nw_state.s_select++;
            if (n_state.nw_state.s_select > 1) {
                n_state.nw_state.s_select = 1;
            }
            if (n_state.nw_state.current_page != countof(w_network) - 2) {
                // tft.fillScreen(TFT_BLACK);
                init();
            }
        }
        break;
    case UP_PRESSED:
        if (n_state.current_network == LORA_PAGE) {
            n_state.nl_state.current_page--;
            if (n_state.nl_state.current_page < 0) {
                n_state.nl_state.current_page = 0;
            }
        } else { // WiFi
            n_state.nw_state.current_page--;
            if (n_state.nw_state.current_page < 0) {
                n_state.nw_state.current_page = 0;
            }
        }
        // tft.fillScreen(TFT_BLACK);
                init();
        break;
    case SELECT_PRESSED:
        if (n_state.current_network == LORA_PAGE) {
            if (n_state.nl_state.is_next) {
                n_state.nl_state.current_page++;
                if (n_state.nl_state.current_page > countof(l_network) - 1) {
                    n_state.nl_state.current_page = countof(l_network) - 1;
                }
                if (n_state.nl_state.current_page != countof(l_network) - 2) {
                    // tft.fillScreen(TFT_BLACK);
                init();
                }
            }
        } else { // WiFi
            if (n_state.nw_state.is_next) {
                n_state.nw_state.current_page++;
                if (n_state.nw_state.current_page > countof(w_network) - 1) {
                    n_state.nw_state.current_page = countof(w_network) - 1;
                }
                if (n_state.nw_state.current_page != countof(w_network) - 2) {
                    // tft.fillScreen(TFT_BLACK);
                init();
                }
            }
        }
        break;
    }

    if (n_state.current_network == 0)
        n_state.nl_state.is_next =
            (this->*l_network[n_state.nl_state.current_page])(n_state.nl_state.s_select);
    else
        n_state.nw_state.is_next =
            (this->*w_network[n_state.nw_state.current_page])(n_state.nw_state.s_select);
    s_data.clear();
    s_data.shrink_to_fit();
    s_data_ready = true;
}

bool UI::Network_1(uint8_t select) {
    TitleDisplay(2);
    NetworkSubtitles(n_state.current_network);

    switch (n_state.current_network) {
        // LoRa
    case 0:
        spr.createSprite(110, 60);
        spr.fillSprite(tft.color565(0, 139, 0));
        spr.setFreeFont(FSSB9);
        spr.setTextColor(TFT_WHITE);
        spr.drawString("LoRa", 30, 26, GFXFF);
        // spr.drawString("(SenseCAP)", 5, 36, GFXFF);
        spr.pushSprite(30, 95);
        spr.deleteSprite();

        spr.createSprite(110, 60);
        spr.fillSprite(TFT_BLACK);
        spr.setFreeFont(FSSB9);
        spr.setTextColor(TFT_WHITE);
        spr.drawString("WiFi", 30, 26, GFXFF);
        // spr.drawString("(Ubidots)", 6, 36, GFXFF);
        spr.pushSprite(180, 95);
        spr.deleteSprite();

        spr.createSprite(320, 50);
        spr.fillSprite(TFT_BLACK);
        spr.setFreeFont(FSS9);
        spr.setTextColor(TFT_WHITE);
        spr.drawString(" Please press the bottom right button to confirm", 2, 6, 2);
        spr.drawString("           your network selection.", 2, 26, 2);
        spr.pushSprite(0, 160);
        spr.deleteSprite();
        break;
        // WiFi
    case 1:
        spr.createSprite(110, 60);
        spr.fillSprite(TFT_BLACK);
        spr.setFreeFont(FSSB9);
        spr.setTextColor(TFT_WHITE);
        spr.drawString("LoRa", 30, 26, GFXFF);
        // spr.drawString("(SenseCAP)", 5, 36, GFXFF);
        spr.pushSprite(30, 95);
        spr.deleteSprite();

        spr.createSprite(110, 60);
        spr.fillSprite(tft.color565(0, 139, 0));
        spr.setFreeFont(FSSB9);
        spr.setTextColor(TFT_WHITE);
        spr.drawString("WiFi", 30, 26, GFXFF);
        // spr.drawString("(Ubidots)", 6, 36, GFXFF);
        spr.pushSprite(180, 95);
        spr.deleteSprite();

        spr.createSprite(320, 50);
        spr.fillSprite(TFT_BLACK);
        spr.setFreeFont(FSS9);
        spr.setTextColor(TFT_WHITE);
        spr.drawString("Please refer to our wiki to configure the info in", 0, 6, 2);
        spr.drawString(" config.txt,then save it and restart the K1100.", 0, 26, 2);
        spr.pushSprite(8, 160);
        spr.deleteSprite();
        break;
    }

    // toDo: Network status
    Status1Display(0);
    return true;
}
// Select Frequency band interface
void UI::NetworkLoRaBandSelect(uint8_t location, struct LoRaBandInfo lbi, uint8_t select) {
    spr.createSprite(340, 22);
    spr.setFreeFont(FSS9);
    spr.setTextColor(TFT_WHITE);
    spr.drawString("Select and confirm LoRaWAN frequency band", 25, 4, 2);
    spr.pushSprite(0, 65);
    spr.deleteSprite();

    spr.createSprite(90, 70);
    if (location == select) {
        spr.fillRect(3, 0, 80, 60, tft.color565(0, 139, 0));
    }
    spr.setFreeFont(FSS9);
    spr.drawString(lbi.type, 5, 0, GFXFF);
    spr.setFreeFont(FSS24);
    spr.drawString(lbi.frequency, 5, 20, GFXFF);

    switch (location) {
    case 0:
        spr.pushSprite(15, 110);
        break;
    case 1:
        spr.pushSprite(115, 110);
        break;
    case 2:
        spr.pushSprite(215, 110);
        break;
    default:;
    }

    spr.deleteSprite();

    if (location == select) {
        spr.createSprite(320, 20);
        spr.setFreeFont(FSS9);
        spr.setTextColor(TFT_WHITE);
        spr.drawString(lbi.type, 80, 0, 2);
        spr.drawString(lbi.frequency, 105, 0, 2);
        spr.drawString("is for", 135, 0, 2);
        spr.drawString(lbi.country, 180, 0, 2);
        spr.pushSprite(0, 195);
        spr.deleteSprite();
    }
}

bool UI::Network_2_0(uint8_t select) {
    bool ret = true;
    TitleDisplay(2);
    // NetworkSubtitles(n_state.current_network);

    if (!cfg.lora_on) { // cfg.is_lorae5_init) {
        for (uint8_t i = 0; i < 3; i++) {
            NetworkLoRaBandSelect(i, lora_band_info[i], select);
        }
        ret = true;
    } else {
        //     spr.createSprite(260, 55);
        //     spr.setTextColor(TFT_WHITE);
        //     spr.drawString("Please Connect the Grove - Wio-E5 to", 3, 6, 2);
        //     spr.drawString("the Grove Connetor on the bottom right", 3, 20, 2);
        //     spr.drawString("side of the screen", 3, 34, 2);
        //     // spr.drawString("(which is included in the kit) to bind", 0, 48, 2);
        //     // spr.drawString("your device to the cloud.", 0, 62, 2);

        //     spr.pushSprite(25, 100);
        //     spr.deleteSprite();
        n_state.nl_state.current_page += 2;
        ret = true;
    }
    Status1Display(0);
    return ret;
}

void UI::NetworkSignal(int16_t signal) {
    spr.setFreeFont(FSS9);
    spr.setTextColor(TFT_WHITE);
    spr.drawString("Signal:", 5, 12, 2);
    if (signal > -70 && signal < 0) {
        spr.fillRect(53, 15, 3, 11, tft.color565(0, 139, 0)); // Four-frame signal
        spr.fillRect(59, 12, 3, 14, tft.color565(0, 139, 0));
        spr.fillRect(65, 9, 3, 17, tft.color565(0, 139, 0));
        spr.fillRect(71, 6, 3, 20, tft.color565(0, 139, 0));
    } else if (signal > -90 && signal < -70) {
        spr.fillRect(53, 15, 3, 11, tft.color565(0, 139, 0)); // Three-frame signal
        spr.fillRect(59, 12, 3, 14, tft.color565(0, 139, 0));
        spr.fillRect(65, 9, 3, 17, tft.color565(0, 139, 0));
        spr.fillRect(71, 6, 3, 20, tft.color565(100, 100, 100));
    } else if (signal > -110 && signal < -90) {
        spr.fillRect(53, 15, 3, 11, tft.color565(0, 139, 0)); // Two-frame signal
        spr.fillRect(59, 12, 3, 14, tft.color565(0, 139, 0));
        spr.fillRect(65, 9, 3, 17, tft.color565(100, 100, 100));
        spr.fillRect(71, 6, 3, 20, tft.color565(100, 100, 100));
    } else if (signal > -130 && signal < -110) {
        spr.fillRect(53, 15, 3, 11, tft.color565(0, 139, 0)); // One frame signal
        spr.fillRect(59, 12, 3, 14, tft.color565(100, 100, 100));
        spr.fillRect(65, 9, 3, 17, tft.color565(100, 100, 100));
        spr.fillRect(71, 6, 3, 20, tft.color565(100, 100, 100));
    } else {
        spr.fillRect(53, 15, 3, 11, tft.color565(140, 42, 42)); // No signal
        spr.fillRect(59, 12, 3, 14, tft.color565(140, 42, 42));
        spr.fillRect(65, 9, 3, 17, tft.color565(140, 42, 42));
        spr.fillRect(71, 6, 3, 20, tft.color565(140, 42, 42));
    }
}

void UI::DisconnectDisplay() {
    spr.createSprite(80, 18);
    spr.fillRect(0, 0, 80, 18, TFT_RED);
    spr.setFreeFont(FSS9);
    spr.setTextColor(TFT_WHITE);
    spr.drawString("Disconnect", 8, 1, 2);
    spr.pushSprite(120, 192);
    spr.deleteSprite();
}

bool UI::Network_2_1(uint8_t select) {
    TitleDisplay(2);
    NetworkSubtitles(n_state.current_network);
    cfg.wifi_on = true;
    cfg.lora_on = false;
    if (cfg.wificonnected) {
        spr.createSprite(200, 60);
        spr.fillSprite(TFT_BLACK);
        spr.setFreeFont(FSS9);
        spr.setTextColor(TFT_WHITE);
        spr.drawString("Connected:", 6, 6, 2);
        spr.drawString("WiFi", 74, 6, 2);

        spr.drawString("SSID:", 6, 26, 2);
        spr.drawString(cfg.ssid, 40, 26, 2);

        spr.drawString("Device Name:", 6, 46, 2);
        spr.drawString(cfg.device_label, 88, 46, 2);

        spr.pushSprite(20, 80);
        spr.deleteSprite();

        spr.createSprite(188, 36);
        NetworkSignal(cfg.wifi_rssi);
        spr.pushSprite(20, 146);
        spr.deleteSprite();

    } else {
        spr.createSprite(200, 60);
        spr.fillSprite(TFT_BLACK);
        spr.setFreeFont(FSS9);
        spr.setTextColor(TFT_WHITE);
        spr.drawString("Waiting for configuration...", 6, 6, 2);
        spr.pushSprite(20, 80);
        spr.deleteSprite();
    }

    DisconnectDisplay();
    Status1Display(0);
    return true;
}

bool UI::Network_3_0(uint8_t select) {
    TitleDisplay(2);
    // NetworkSubtitles(n_state.current_network);
    if (!cfg.lora_on) {
        cfg.lora_frequency = lora_band_info[select].band;
        cfg.wifi_on        = false;
        spr.createSprite(300, 80);
        spr.setTextColor(TFT_WHITE);
        spr.drawString("Please download and register an account", 0, 6, 2);
        spr.drawString("on our SenseCAP Mate APP, then scan the", 0, 20, 2);
        spr.drawString("QR code on the back of Grove-Wio E5", 0, 34, 2);
        spr.drawString("(which is included in the kit) to bind", 0, 48, 2);
        spr.drawString("your device to the cloud.", 0, 62, 2);
        spr.pushSprite(25, 80);
        spr.deleteSprite();

        spr.createSprite(80, 20);
        spr.fillRect(0, 0, 80, 20, 0x03ff);
        spr.setFreeFont(FSS9);
        spr.setTextColor(TFT_WHITE);
        spr.drawString("Continue", 12, 2, 2);
        spr.pushSprite(120, 180);
        spr.deleteSprite();
        Status1Display(0);
    } else {
        n_state.nl_state.current_page -= 2;
    }
    return true;
}

bool UI::Network_3_1(uint8_t select) {
    spr.createSprite(160, 80);
    spr.fillRect(0, 0, 160, 80, TFT_WHITE);
    spr.setFreeFont(FSS9);
    spr.setTextColor(TFT_BLACK);
    spr.drawString("Disconnected?", 26, 10, GFXFF);
    if (select == 0) {
        spr.fillRect(24, 42, 40, 24, TFT_GREEN);
    } else {
        spr.fillRect(94, 42, 40, 24, TFT_GREEN);
    }

    spr.fillRect(26, 44, 36, 20, TFT_RED);
    spr.drawString("Yes", 30, 46, 2);

    spr.fillRect(96, 44, 36, 20, 0x02ff);
    spr.drawString("No", 106, 46, 2);

    spr.pushSprite(80, 80);
    spr.deleteSprite();
    return true;
}
bool UI::Network_4_1(uint8_t select) {
    if (select == 0) {
        cfg.wifi_on = false;
        n_state.nw_state.current_page -= 3;
    } else
        n_state.nw_state.current_page -= 2;
}

bool UI::Network_4_0(uint8_t select) {
    TitleDisplay(2);
    NetworkSubtitles(n_state.current_network);
    cfg.lora_on = true;
    spr.createSprite(188, 64);
    spr.setTextColor(TFT_WHITE);
    spr.drawString("Connected: LoRa  ", 5, 3.8 * FONT_ROW_HEIGHT - 85, 2);
    spr.drawString("Total Send:", 5, 4.8 * FONT_ROW_HEIGHT - 85, 2);
    spr.drawString("packets", 115, 4.8 * FONT_ROW_HEIGHT - 85, 2);
    spr.drawString("Succeed:", 5, 5.8 * FONT_ROW_HEIGHT - 85, 2);
    spr.drawString("packets", 105, 5.8 * FONT_ROW_HEIGHT - 85, 2);

    spr.setFreeFont(FSSB9);
    spr.setTextColor(tft.color565(0, 139, 0));
    spr.drawString(String(cfg.lora_fcnt), 80, 4.8 * FONT_ROW_HEIGHT - 85,
                   2); // Show total number of packages issued
    spr.drawString(String(cfg.lora_sucess_cnt), 70, 5.8 * FONT_ROW_HEIGHT - 85,
                   2); // Shows the number of successful deliveries

    spr.pushSprite(20, 96);
    spr.deleteSprite();

    spr.createSprite(188, 30);
    NetworkSignal(cfg.lora_rssi);
    spr.pushSprite(20, 160);
    spr.deleteSprite();

    spr.createSprite(90, 75);
    switch (cfg.lora_status) {
    case LORA_INIT_START:
        spr.fillCircle(265 - 220, 4.9 * FONT_ROW_HEIGHT - 90, 10,
                       tft.color565(139, 139, 0)); // Data transmission status: init failed
        spr.setTextColor(TFT_YELLOW);
        spr.drawString("LoRaWAN INIT", 220 - 220, 5.8 * FONT_ROW_HEIGHT - 90, 2);
        break;
    case LORA_INIT_FAILED:
        spr.fillCircle(265 - 220, 4.9 * FONT_ROW_HEIGHT - 90, 10,
                       tft.color565(34, 139, 139)); // Data transmission status: init failed
        spr.setTextColor(TFT_RED);
        spr.drawString("LoRaWAN INIT", 220 - 220, 5.8 * FONT_ROW_HEIGHT - 90, 2);
        spr.drawString("Failed", 250 - 220, 6.6 * FONT_ROW_HEIGHT - 90, 2);
        break;
    case LORA_INIT_SUCCESS:
        spr.fillCircle(265 - 220, 4.9 * FONT_ROW_HEIGHT - 90, 10,
                       tft.color565(0, 139, 0)); // Data transmission status: init success
        spr.setTextColor(TFT_GREEN);
        spr.drawString("LoRaWAN INIT", 220 - 220, 5.8 * FONT_ROW_HEIGHT - 90, 2);
        spr.drawString("Success", 250 - 220, 6.6 * FONT_ROW_HEIGHT - 90, 2);
        break;
    case LORA_JOIN_FAILED:
        spr.fillCircle(265 - 220, 4.9 * FONT_ROW_HEIGHT - 90, 10,
                       tft.color565(160, 34, 34)); // Data transmission status: join failed
        spr.setTextColor(TFT_RED);
        spr.drawString("Join LoRaWAN", 220 - 220, 5.8 * FONT_ROW_HEIGHT - 90, 2);
        spr.drawString("Failed", 250 - 220, 6.6 * FONT_ROW_HEIGHT - 90, 2);
        break;
    case LORA_JOIN_SUCCESS:
        spr.fillCircle(265 - 220, 4.9 * FONT_ROW_HEIGHT - 90, 10,
                       tft.color565(0, 139, 0)); // Data transmission status: join success
        spr.setTextColor(TFT_GREEN);
        spr.drawString("Join LoRaWAN", 220 - 220, 5.8 * FONT_ROW_HEIGHT - 90, 2);
        spr.drawString("Success", 250 - 220, 6.6 * FONT_ROW_HEIGHT - 90, 2);
        break;
    case LORA_SEND_FAILED:
        spr.fillCircle(265 - 220, 4.9 * FONT_ROW_HEIGHT - 90, 10,
                       tft.color565(255, 165, 0)); // Data transmission status: Packet loss
        spr.setTextColor(TFT_RED);
        spr.drawString("Send", 253 - 220, 5.8 * FONT_ROW_HEIGHT - 90, 2);
        spr.drawString("Failed", 250 - 220, 6.6 * FONT_ROW_HEIGHT - 90, 2);
        break;
    case LORA_SEND_SUCCESS:
        spr.fillCircle(265 - 220, 4.9 * FONT_ROW_HEIGHT - 90, 10,
                       tft.color565(0, 139, 0)); // Data transmission status: Packet success
        spr.setTextColor(TFT_GREEN);
        spr.drawString("Send", 253 - 220, 5.8 * FONT_ROW_HEIGHT - 90, 2);
        spr.drawString("Success", 250 - 220, 6.6 * FONT_ROW_HEIGHT - 90, 2);
        break;
    default:
        break;
    }
    spr.pushSprite(208, 96);
    spr.deleteSprite();

    DisconnectDisplay();
    Status1Display(0);
    return true;
}

bool UI::Network_5_0(uint8_t select) {
    spr.createSprite(160, 80);
    spr.fillRect(0, 0, 160, 80, TFT_WHITE);
    spr.setFreeFont(FSS9);
    spr.setTextColor(TFT_BLACK);
    spr.drawString("Disconnected?", 26, 10, GFXFF);
    if (select == 0) {
        spr.fillRect(24, 42, 40, 24, TFT_GREEN);
    } else {
        spr.fillRect(94, 42, 40, 24, TFT_GREEN);
    }

    spr.fillRect(26, 44, 36, 20, TFT_RED);
    spr.drawString("Yes", 30, 46, 2);

    spr.fillRect(96, 44, 36, 20, 0x02ff);
    spr.drawString("No", 106, 46, 2);

    spr.pushSprite(80, 80);
    spr.deleteSprite();
    return true;
}

bool UI::Network_6_0(uint8_t select) {
    if (select == 0) {
        cfg.lora_on = false;
        n_state.nl_state.current_page -= 5;
    } else
        n_state.nl_state.current_page -= 2;
}

void UI::ProcessPageManager(uint8_t key) {
    switch (key) {
    case LEFT_PRESSED:
        p_state.s_select--;
        if (p_state.s_select < 0) {
            p_state.s_select = 0;
        }
        // tft.fillScreen(TFT_BLACK);
                init();
        break;
    case RIGHT_PRESSED:
        p_state.s_select++;
        // 只有两个选择
        if (p_state.s_select > 2) {
            p_state.s_select = 2;
        }
        // tft.fillScreen(TFT_BLACK);
                init();
        break;
    case UP_PRESSED:
        p_state.current_page--;
        if (p_state.current_page < 0) {
            p_state.current_page = 0;
        }
        // tft.fillScreen(TFT_BLACK);
                init();
        break;
    case SELECT_PRESSED:
        if (p_state.is_next) {
            p_state.current_page++;
            if (p_state.current_page > countof(process) - 1) {
                p_state.current_page = countof(process) - 1;
            }
        }
        // tft.fillScreen(TFT_BLACK);
                init();
        break;
    }
    p_state.is_next = (this->*process[p_state.current_page])(p_state.s_select);
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

bool UI::Process_1(uint8_t select) {
    TitleDisplay(1);

    ProcessSubTitle(select);

    switch (select) {
    // Vision AI real-time analysis
    case 0:
        // 270*80 = 21600
        spr.createSprite(270, 80);
        spr.setFreeFont(FSSB9);
        spr.setTextColor(TFT_WHITE);

        spr.setFreeFont(FSS9);
        spr.setTextColor(TFT_WHITE);

        spr.fillRect(52, 0, 75, 60, tft.color565(0, 139, 0)); // Green
        spr.drawString("Vision AI", 54, 0, GFXFF);
        spr.drawString("real-time", 54, 22, GFXFF);
        spr.drawString("analysis", 54, 44, GFXFF);

        spr.drawString("TinyML", 180, 0, GFXFF);
        spr.drawString("Example", 180, 25, GFXFF);

        // spr.drawString("Data", 205, 0, GFXFF);
        // spr.drawString("Filter (In ", 205, 20, GFXFF);
        // spr.drawString("Develop", 205, 40, GFXFF);
        // spr.drawString("ment)", 205, 60, GFXFF);

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

        spr.drawString("Vision AI", 54, 0, GFXFF);
        spr.drawString("real-time", 54, 22, GFXFF);
        spr.drawString("analysis", 54, 44, GFXFF);

        spr.fillRect(178, 0, 75, 40, tft.color565(0, 139, 0));
        spr.drawString("TinyML", 180, 0, GFXFF);
        spr.drawString("Example", 180, 25, GFXFF);

        // spr.drawString("Data", 205, 0, GFXFF);
        // spr.drawString("Filter (In ", 205, 20, GFXFF);
        // spr.drawString("Develop", 205, 40, GFXFF);
        // spr.drawString("ment)", 205, 60, GFXFF);

        spr.pushSprite(5, 100);
        spr.deleteSprite();
        break;

    // toDo: Data Filter
    case 2:
        p_state.s_select = 1;
        break;
    }
    // toDo: Network status
    Status1Display(0);
    return true;
}

bool UI::Process_2(uint8_t select) {
    int  i = 0;
    char buf[16];
    TitleDisplay(1);
    ProcessSubTitle(select);
    switch (select) {
    // Vision AI real-time analysis
    case 0: {
        // check all data if vision ai is running
        log_ready = false;
        if (a_log.size() == 0) {
            log_ready = true;
            break;
        }
        // LogMemoryUsage(__FUNCTION__);
        // spr.createSprite(320, 130);
        tft.setFreeFont(FSS9);
        for (auto data : a_log) {
            sprintf(buf, "[%02d:%02d:%02d]:", data.time / 1000 / 60 / 60,
                    data.time / 1000 / 60 % 60, data.time / 1000 % 60);
            // tft.setTextPadding(130);
            // tft.fillRect(20, 81 + i * 11, 280, 9, TFT_BLACK);
            tft.setTextColor(TFT_GREEN, TFT_BLACK);
            tft.drawString(buf, 20, 80 + i * 13, 2);
            tft.setTextColor(TFT_WHITE, TFT_BLACK);
            tft.drawString(data.data, 100, 80 + i * 13, 2);
            i++;
        }
        // spr.pushSprite(0, 80);
        // LogMemoryUsage(__FUNCTION__);
        // spr.deleteSprite();
        log_ready = true;
        break;
    }
    // TinyML Example
    case 1: {
        spr.createSprite(130, 130);
        spr.setTextColor(TFT_WHITE);

        double PIXELL = 3;

        spr.fillRect(15, 0, 113, 113, TFT_WHITE);
        QRCode   qrcode;
        uint8_t *qrcodeData = (uint8_t *)malloc(qrcode_getBufferSize(5)); // 1376/8
        qrcode_initText(&qrcode, qrcodeData, 5, 0,
                        "https://wiki.seeedstudio.com/K1100-Getting-Started/#tinyml-section");
        for (uint8_t y = 0; y < qrcode.size; y++) {
            // Each horizontal module
            for (uint8_t x = 0; x < qrcode.size; x++) {
                if (qrcode_getModule(&qrcode, x, y))
                    spr.fillRect(x * PIXELL + 15 + 1, y * PIXELL + 1, PIXELL, PIXELL, TFT_BLACK);
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
        LogMemoryUsage(__FUNCTION__);
        spr.deleteSprite();
        break;
    }
    // toDo: Data Filter
    case 2:
        p_state.s_select = 1;
        break;
    }
    // toDo: Network status
    Status1Display(0);
}

void UI::SensePageManager(uint8_t key) {
    uint8_t Sensor_num;
    switch (key) {
    case LEFT_PRESSED:
        s_state.s_select--;
        if (s_state.s_select < 0) {
            s_state.s_select = 0;
        }
        init();
        // if (s_state.current_page == 1)
            // tft.fillScreen(TFT_BLACK);
        break;
    case RIGHT_PRESSED:
        s_state.s_select++;
        Sensor_num = s_data.size() - 1;
        if (s_data.size() < SENSOR_NUM_MAX)
            Sensor_num = SENSOR_NUM_MAX - 1;
        if (s_state.s_select > Sensor_num) {
            s_state.s_select = Sensor_num;
        }
        init();
        // if (s_state.current_page == 1)
            // tft.fillScreen(TFT_BLACK);
        break;
    case UP_PRESSED:
        s_state.current_page--;
        if (s_state.current_page < 0) {
            s_state.current_page = 0;
        }
        init();
        // tft.fillScreen(TFT_BLACK);
        break;
    case SELECT_PRESSED:
        if (s_state.is_next) {
            s_state.current_page++;
            if (s_state.current_page > countof(sense) - 1) {
                // save sensor data to SD card
                if (cfg.sensor_save_flag & 1 << s_data[s_state.s_select].id) {
                    cfg.sensor_save_flag &= ~(1 << s_data[s_state.s_select].id);
                } else {
                    cfg.sensor_save_flag |= 1 << s_data[s_state.s_select].id;
                }
                s_state.current_page = countof(sense) - 1;
            }
        }
        init();
        // tft.fillScreen(TFT_BLACK);
        break;
    }

    // LOGSS.printf("\r\n>>>>>UI Sensor number: %d  %d  %d\r\n", s_state.s_select ,
    // s_data.size(), key);
    s_state.is_next = (this->*sense[s_state.current_page])(s_state.s_select);
}

void UI::SensorSubTitle(String value, int si) {
    tft.setFreeFont(FSSB9);
    tft.setTextColor(TFT_WHITE, tft.color565(100, 100, 100));
    if (value.length() > 7) {
        tft.drawString(value, 2*(13-value.length())+(20+si*100), 60, FONT2);
    } else {
        tft.drawString(value, 5*(8-value.length())+(20+si*100), 60, GFXFF);
    }
}

// display sensor chart subtitle 
void UI::SensorSubTitle2(String value) {
    // tft.createSprite(300, 25);
    tft.setFreeFont(FSSB9);
    tft.setTextColor(TFT_WHITE, tft.color565(100, 100, 100));
    if (value.length() > 7) {
        tft.drawString(value, 2*(13-value.length())+120, 50, FONT2);
    } else {
        tft.drawString(value, 5*(8-value.length())+120, 50, GFXFF);
    }
}

void UI::SensorUnit(String value, int si) {
    tft.setFreeFont(FSS9);
    tft.setTextColor(TFT_WHITE);
    if (value.length() > 7) {
        tft.drawString(value, 2*(13-value.length())+(20+si*100), 175, FONT2);
    } else {
        tft.drawString(value, 5*(8-value.length())+(20+si*100), 175, GFXFF);
    }
}

void UI::SensorADDDisplay(uint8_t is_backgroud) {

    if (is_backgroud) {
        // tft.fillRect(220, 85, 90, 120, tft.color565(0, 139, 0));
    }

    tft.setFreeFont(FSS9);
    tft.setTextColor(TFT_WHITE);
    tft.drawString("ADD", 234, 87, 4);

    tft.fillRect(240, 82+1.5*FONT_ROW_HEIGHT, 40, 40, TFT_WHITE);
    tft.fillRect(258, 82+1.5*FONT_ROW_HEIGHT+6, 3, 26, TFT_BLACK);
    tft.fillRect(247, 82+1.5*FONT_ROW_HEIGHT+18, 26, 3, TFT_BLACK);

}

void UI::SensorPageState(int pages_num, int page_select) {

    LogMemoryUsage(__FUNCTION__);
    // spr.createSprite(340, 10);
    // 问题
    int *page_location = new int[pages_num];

    int temp = 0;

    for (int i = 0; i < pages_num; i++) {
        page_location[temp] = 140 - pages_num / 2 * 10 * (1 + pages_num % 2) + i * 20;
        temp++;
    }

    for (int i = 0; i < pages_num; i++) {
        if (i == page_select)
            tft.fillCircle(page_location[i], 206, 3, tft.color565(0, 193, 255));
        else
            tft.fillCircle(page_location[i], 206, 3, tft.color565(220, 220, 220));
    }
    delete page_location;
    // spr.pushSprite(0, 200);
    // LogMemoryUsage(__FUNCTION__);
    // spr.deleteSprite();
}

// 显示Sensor首页
bool UI::Sensor_1(uint8_t select) {
    bool           ret;
    uint8_t        sense_display_num = 0, len = 0;
    uint16_t       bg_color;
    static uint8_t index  = 0; // 指示页面最左侧传感器的序号
    static uint8_t offset = 3; // 指示当前选中传感器的位置偏移
    s_data_ready          = false;
    if (s_data.size() == 0) {
        s_data_ready = true;
        return false;
    }
    TitleDisplay(0);
    //检查是否接入grove传感器，并显示提示信息
    if (select < s_data.size()) {
        Status2Display(0xff);
        ret = true;
    } else {
        Status2Display(0);
        ret = false;
    }
    //获取显示数据的位置
    if (select >= index + 3) { //所选传感器在当前页之后
        index += 1; 
        tft.fillRect(0, 60, 320, 130, TFT_BLACK);
        tft.fillRoundRect(20+(select-index)*100, 60, 90, 130, 10, tft.color565(0, 139, 0));
    }
    else if (select < index) { //所选传感器在当前页之前
        index = select;
        tft.fillRect(0, 60, 320, 130, TFT_BLACK);
        tft.fillRoundRect(20+(select-index)*100, 60, 90, 130, 10, tft.color565(0, 139, 0));
    }
    if(offset != select - index){
        tft.fillRect(0, 60, 320, 130, TFT_BLACK);
        tft.fillRoundRect(20+(select-index)*100, 60, 90, 130, 10, tft.color565(0, 139, 0));
    }
    offset = select - index;

    //只显示select-1, select和select+1的数据
    for (int si = 0; si < 3; si++) { // si : 指示传感器的位置偏移
        // spr.createSprite(90, 130);
        // 背景层，绿色矩形高亮选择的数据
        if (index+si == select) {
            bg_color = tft.color565(0, 139, 0);
            // tft.fillRoundRect(20+si*100, 60, 90, 130, 10, tft.color565(0, 139, 0));
        }
        else{
            bg_color = TFT_BLACK;
        }

        if (index+si >= s_data.size()) {
            SensorADDDisplay(index+si == select); // Display ADD when no grove sensor
        } 
        else {
            SensorSubTitle(s_data[index+si].name, si); // Display the sensor name
            SensorUnit(s_data[index+si].data_unit, si); // Display the sensor unit
            //显示数据： 0正常显示 1显示平均值
            tft.setFreeFont(FSS9);
            tft.setTextColor(TFT_WHITE, bg_color);
            if (s_data[index+si].ui_type == SENSOR_UI_TYPE_NORMAL) {
                //一次只显示4个数据，每个测量数据4个byte
                if (s_data[index+si].size > 4 * 4)
                    sense_display_num = 4 * 4;
                else
                    sense_display_num = s_data[index+si].size;
                //把框分成4行，每行显示一个数据
                for (int i = 0; i < sense_display_num; i += 4) {
                    int32_t dd = ((uint8_t *)s_data[index+si].data)[i] << 0 |
                                 ((uint8_t *)s_data[index+si].data)[i + 1] << 8 |
                                 ((uint8_t *)s_data[index+si].data)[i + 2] << 16 |
                                 ((uint8_t *)s_data[index+si].data)[i + 3] << 24;
                    len = String(dd).length();
                    if (len >= sense_display_num)
                        len = sense_display_num - 1;
                    if (s_data[index+si].data_type == SENSOR_DATA_TYPE_INT32)
                        tft.drawString(String(dd), 2*(sense_display_num-len)+(20+si*100),
                                        30+24*i/4+60, 2*(4-sense_display_num/4));
                    else if (s_data[index+si].data_type == SENSOR_DATA_TYPE_FLOAT)
                        tft.drawFloat((float)dd/100, 2, 2*(sense_display_num-len)+(20+si*100),
                                        30+24*i/4+60, 2*(4 - sense_display_num / 4));
                }
            } else {
                int32_t temp = 0;
                for (int i = 0; i < s_data[index+si].size; i += 4) {
                    temp += *(int32_t *)(s_data[index+si].data + i);
                }
                temp /= s_data[index+si].size / 4;
                tft.drawString(String(s_data[index+si].size/4), 2+(20+si*100), 30+60, 4);
                if (s_data[index+si].data_type == SENSOR_DATA_TYPE_INT32)
                    tft.drawString(String(temp), 2+(20+si*100), 30+24+60, 4);
                else if (s_data[index+si].data_type == SENSOR_DATA_TYPE_FLOAT)
                    tft.drawFloat((float)temp/100, 2, 2+(20+si*100), 30+24+60, 4);
            }
        }
        //根据数据的index，显示不同的位置，一个页面只能显示三个，页面不够补+号。
        // spr.pushSprite((20+si*100), 60);
        LogMemoryUsage(__FUNCTION__);
        // spr.deleteSprite();
    }
    // SensorADDDisplay(1);
    SensorPageState(s_data.size()/3+1, select/3);
    s_data_ready = true;
    Status1Display(0); //左下角显示连接状态
    return ret;
}

bool UI::Sensor_2(uint8_t select) {
    static uint8_t select_old = 0;
    uint16_t       line_col[] = {TFT_GREEN, TFT_RED, TFT_BLUE, TFT_YELLOW};
    uint8_t        data_num   = 0;

    if (select > s_data.size() - 1)
        select = s_data.size() - 1;
    TitleDisplay(0);
    // Display the sensor name
    SensorSubTitle2(s_data[select].name);

    tft.fillRect(18, 78, 24, 90, TFT_WHITE);

    // 85 * 260 = 22100
    auto content = line_chart(20, 80); //(x,y) where the line graph begins
    data_num     = s_data[select].size / 4;
    if (data_num > DRAW_LINE_MAX_NUM)
        data_num = DRAW_LINE_MAX_NUM;

    for (int i = 0; i < data_num; i++) {
        // clear the old line
        if (select_old != select) {
            std::queue<double> empty;
            swap(empty, line_chart_data[i]);
        }
        if (line_chart_data[i].size() > LINE_DATA_MAX_SIZE) // keep the old line chart front
        {
            line_chart_data[i].pop(); // this is used to remove the first read variable
        }
        line_chart_data[i].push(((int32_t *)s_data[select].data)[i]);

        content.height(85)
            .width(260)
            .based_on(0.0)             // Starting point of y-axis, must be a float
            .show_circle(false)        // drawing a cirle at each point, default is on.
            .value(line_chart_data[i]) // passing through the data to line graph
            .max_size(LINE_DATA_MAX_SIZE)
            .color(line_col[i]) // Setting the color for the line
                                //        .backgroud(tft.color565(0,0,0)) // Setting the color for
                                //        the backgroud
            .backgroud(tft.color565(0, 0, 0))
            .draw(&tft);
        LogMemoryUsage(__FUNCTION__);
    }
    select_old = select;
    SensorPageState(s_data.size() / 3 + 1, select / 3);
    Status1Display(0);
    return true;
}
void UI::SensorSwitchButton(uint8_t button) {
    // spr.createSprite(320, 100);
    tft.setTextColor(TFT_WHITE);
    tft.drawString("Save to TF card ", 32, 125, FONT4);
    unsigned int gg_switch_state_color[4] = {tft.color565(211, 211, 211), TFT_BLUE,
                                             tft.color565(201, 201, 201),
                                             tft.color565(65, 105, 235)};
    if (button == 1) {
        tft.fillCircle(240, 130, 14/2, gg_switch_state_color[button + 2]);
        tft.fillRect(240, 130 - 14/2, 22, 14 + 1, gg_switch_state_color[button + 2]);
        tft.fillCircle(240 + 22, 130, 14, gg_switch_state_color[button]);
    } else {
        tft.fillCircle(240 + 22, 130, 14/2, gg_switch_state_color[button + 2]);
        tft.fillRect(240, 130 - 14/2, 22, 14 + 1, gg_switch_state_color[button + 2]);
        tft.fillCircle(240, 130, 14, gg_switch_state_color[button]);
    }
    // spr.pushSprite(0, 75);
    // spr.deleteSprite();
}

bool UI::Sensor_3(uint8_t select) {
    // remember button state to refreshes
    // static uint8_t button_flag = 0; 
    // if(button_flag != cfg.sensor_save_flag){
    //     button_flag = cfg.sensor_save_flag;

    // }

    if (select > s_data.size() - 1)
        select = s_data.size() - 1;
    TitleDisplay(0);
    // Display the sensor name
    SensorSubTitle2(s_data[select].name);
    if (cfg.sensor_save_flag == 0) {
        Status2Display(0x5);
        SensorSwitchButton(0);
    } else {
        if (cfg.sd_status == 1) {

            if (cfg.sensor_save_flag & 1 << s_data[select].id) {
                SensorSwitchButton(1);
                Status2Display(0x3);
            } else {
                SensorSwitchButton(0);
                Status2Display(0xff);
            }
        } else if (cfg.sd_status == 2) {
            Status2Display(0x1);
            SensorSwitchButton(0);
        } else {
            Status2Display(0x4);
            SensorSwitchButton(0);
        }
    }
    Status1Display(0);
    return true;
}