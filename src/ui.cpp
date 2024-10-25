#include "ui.h"
#include "qrcode.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define countof(a) (sizeof(a) / sizeof(*(a)))
extern void LogMemoryUsage(const char *s);
extern void LogTaskTrace(void);

// inline function, 4byte uint8_t to float
void UI::uint8_to_float(uint8_t *data, float *destination) {
    uint32_t value =
        data[0] | ((uint32_t)data[1]) << 8 | ((uint32_t)data[2]) << 16 | ((uint32_t)data[1]) << 24;
    *reinterpret_cast<uint32_t *>(destination) = value;
}

UI::UI(TFT_eSPI &lcd, SysConfig &config, Message &m1)
    : Thread("UIThread", 128 * 6, 3), tft(lcd), cfg(config), btnMail(m1) {
    Start();
};

void UI::Run() {
    uint8_t nums;
    uint8_t keys        = 2;
    page.mainstate      = SENSEPAGE;
    page.key            = 0;
    s_data_ready        = true;
    layout_refresh      = true;
    data_refresh        = true;

    // remember the previous status to refresh on change
    uint8_t status_pre[4] = {0,0,0,0};
    while (true) {
        // Whether to rotate the screen
        if (rotate_status != rotate_flag) {
            rotate_status = rotate_flag;
            layout_refresh = true;
            if (rotate_flag == true)
                tft.setRotation(1);
            else
                tft.setRotation(3);
        }
        // detect button event for refresh
        nums = btnMail.Receive(&buff, 256, 0);
        if (nums > 0) {
            LOGSS.printf("btn Receive: %d %d\r\n", nums, buff[0]);
            keys = buff[0] - 1;
            layout_refresh = true;
        } else {
            keys = NONE_PRESSED;
        }
        // detect status change for refresh
        if (status_pre[0] != s_data.size()) { // grove num change?
            status_pre[0] = s_data.size();
            layout_refresh = true;
        }
        if (status_pre[1] != cfg.sd_status) { // sd status change?
            status_pre[1] = cfg.sd_status;
            layout_refresh = true;
        }
        if (status_pre[2] != cfg.wificonnected) {
            status_pre[2] = cfg.wificonnected;
            layout_refresh = true;
        };
        if (status_pre[3] != cfg.lora_status) {
            status_pre[3] = cfg.lora_status;
            layout_refresh = true;
        };


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
    // Data is not updated when Ui refreshes
    if (s_data_ready) {
        s_data.clear();
        s_data.shrink_to_fit();
        for (auto data : d) {
            s_data.push_back(*data);
            // Whether to rotate the screen
            if (data->name == "IMU") {
                if (((int32_t *)data->data)[0] > 50)
                    rotate_flag = false;
                if (((int32_t *)data->data)[0] < -50)
                    rotate_flag = true;
            }
        }
        data_refresh = true;
    }
}

void UI::UIPushLog(std::vector<log_data> d) {
    // Data is not updated when Ui refreshes
    if (log_ready) {
        int log_num = d.size();
        // 最多只显示11条日志
        if (log_num > SHOW_LOG_MAX_SIZE)
            log_num = SHOW_LOG_MAX_SIZE;
        a_log.clear();
        a_log.shrink_to_fit();
        for (int i = 0; i < log_num; i++)
            a_log.push_back(d.at(d.size() - log_num + i));
        data_refresh = true;
    }
}

#define PIXEL 4 // Width of one letter
#define LEFT_SIDE 70
#define HIGHT_SIDE 47
const static unsigned int FONT_ROW_HEIGHT = 22; // The height of a letter

void UI::PageMangent(uint8_t key) {
    if(layout_refresh){
        tft.fillScreen(TFT_BLACK);
    }
    else if(!data_refresh){
        return;
    }
    

    if (key < 3) { // switch main state
        page.mainstate = (page_state)key;
        key            = NONE_PRESSED;
    }

    // call pagemanager and refresh a page
    (this->*get_page[page.mainstate])(key);

    layout_refresh = false;
    data_refresh = false;
}

// todo: Not enabled, to be improved
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
        // only two options
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

void UI::NetworkPageManager(uint8_t keys) {
    if (!layout_refresh) //this page not included data refresh
        return;
    switch (keys) {
    case LEFT_PRESSED:
        if ((u_state.current_page == HOME_S) || (u_state.current_page == LORABAND_S)
            || (u_state.current_page == DISCONNECT_S))
            u_state.s_select -= (u_state.s_select > 0) ? 1 : 0;
        break;
    case RIGHT_PRESSED:
        if (u_state.current_page == LORABAND_S)
            u_state.s_select += (u_state.s_select < 2) ? 1 : 0;
        else
            u_state.s_select += (u_state.s_select < 1) ? 1 : 0;
        break;
    case UP_PRESSED:
        if (u_state.current_page == LORABAND_S) {
            u_state.s_select = 0;
            u_state.current_page = HOME_S;
        }
        else
            u_state.current_page -= (u_state.current_page > HOME_S) ? 1 : 0;
        break;
    case SELECT_PRESSED:
        switch (u_state.current_page)
        {
#ifdef CN_VER
        case HOME_S:
            cfg.wifi_on = true;
            cfg.cloud = (u_state.s_select == 0)?CLOUD_UBIDOTS:
                                                CLOUD_AZURE;
            u_state.current_page = CONNECT;
            break;
#else
        case HOME_S:
            if (u_state.s_select == 1) {
                cfg.lora_on = false;
                cfg.wifi_on = true;
                u_state.current_page = CONNECT;
            }
            else
                u_state.current_page = (cfg.lora_on) ? CONNECT : LORABAND_S;
            break;
#endif
        case DISCONNECT_S:
            if (u_state.s_select == 0) {
                cfg.lora_on = cfg.wifi_on = false;
                u_state.current_page = HOME_S;
            }
            else
                u_state.current_page = CONNECT;
            break;
        case LORACONFIRM:
            cfg.lora_on = true;
            cfg.wifi_on = false;
            u_state.current_page = CONNECT;
            break;
        default:
            u_state.current_page += 1;
            break;
        }
        break;
    }
    // LOGSS.printf("page switch: %d %d\r\n", u_state.current_page, u_state.s_select);
    u_state.is_next = (this->*uplink[u_state.current_page])(u_state.s_select);
}

bool UI::Network_Home(uint8_t select) {
    Widget_Title(2);
    Label_Network();
    tft.setFreeFont(FSSB9);
    tft.setTextColor(TFT_WHITE);
#ifdef CN_VER
    tft.fillRect(30+150*select, 95, 110, 60, tft.color565(0, 139, 0));
    tft.drawCentreString("Ubidots", 30+55, 121, GFXFF);
    tft.drawCentreString("Azure", 30+55+150, 121, GFXFF);
    String tab = (select == 0) ? "Ubidots" : "Azure";
    tft.drawString("Please refer to our wiki to configure the info in", 8, 166, 2);
    tft.drawString(" config.txt,then save it and restart the K1101.", 8, 186, 2);
#else
    tft.fillRect(30+150*select, 95, 110, 60, tft.color565(0, 139, 0));
    tft.drawCentreString("LoRa", 30+55, 121, GFXFF);
    tft.drawCentreString("WiFi", 30+55+150, 121, GFXFF);
    String tab = (select == 0) ? "LoRa" : "WiFi";
    if (select == 0) {
        tft.drawString(" Please press the bottom right button to confirm", 2, 166, 2);
        tft.drawString("           your network selection.", 2, 186, 2);
    } else {
        tft.drawString("Please refer to our wiki to configure the info in", 8, 166, 2);
        tft.drawString(" config.txt,then save it and restart the K1100.", 8, 186, 2);
    }
#endif
    Label_Subtitle(tab);
    return true;
}

bool UI::Network_Connect(uint8_t select) {
    Widget_Title(2);
    Label_CentreBtn("Disconnect", TFT_RED);
    Label_Network();
    if (cfg.wifi_on) {
        Label_Subtitle("WiFi");
        tft.setFreeFont(FSS9);
        tft.setTextColor(TFT_WHITE);
        if (cfg.wificonnected) {
            tft.drawString("Connected:", 26, 86, 2);
            tft.drawString("WiFi", 100, 86, 2);
            tft.drawString("SSID:", 26, 106, 2);
            tft.drawString(cfg.ssid, 60, 106, 2);
            tft.drawString("Device Name:", 26, 126, 2);
            tft.drawString(cfg.device_label, 108, 126, 2);
            tft.drawString("Signal:", 26, 152, 2);
            Widget_Signal(cfg.wifi_rssi, 0, 0);
        } else {
            tft.drawString("Waiting for configuration...", 26, 86, 2);
        }
    }
    if (cfg.lora_on) {
        Label_Subtitle("LoRa");
        tft.setTextColor(TFT_WHITE);
        tft.drawString("Connected: LoRa  ", 25, 4 * FONT_ROW_HEIGHT, 2);
        tft.drawString("Total Send:      packets", 25, 5 * FONT_ROW_HEIGHT, 2);
        tft.drawString("Succeed:         packets", 25, 6 * FONT_ROW_HEIGHT, 2);
        tft.drawString("Signal:", 25, 7 * FONT_ROW_HEIGHT + 5, 2);
        tft.setFreeFont(FSSB9);
        tft.setTextColor(tft.color565(0, 139, 0));
        tft.drawString(String(cfg.lora_fcnt), 100, 4.5 * FONT_ROW_HEIGHT + 11, 2); // Show total number of packages issued
        tft.drawString(String(cfg.lora_sucess_cnt), 100, 5.5 * FONT_ROW_HEIGHT + 11, 2); // Shows the number of successful deliveries
        Widget_LoraState(0, 0);
        Widget_Signal(cfg.lora_rssi, 2, 7);
    }
    return true;
}

bool UI::Network_LoRa_Band(uint8_t select) {
    Widget_Title(2);
    Label_Network();
    for (uint8_t i = 0; i < 3; i++) {
        struct LoRaBandInfo lbi = lora_band_info[i];
        int len = strlen(lbi.type) + strlen(lbi.frequency) + strlen(lbi.country);
        char *freq_info = new char[len+9];
        sprintf(freq_info,"%s%s is for %s", lbi.type, lbi.frequency, lbi.country);
        if (i == select) {
            tft.fillRect(20 + i * 100, 110, 80, 60, tft.color565(0, 139, 0));
            tft.drawCentreString(freq_info, 160, 195, 2);
        }
        tft.setFreeFont(FSS9);
        tft.setTextColor(TFT_WHITE);
        tft.drawCentreString("Select and confirm LoRaWAN frequency band", 160, 69, 2);
        tft.drawString(lbi.type, 20 + i * 100, 110, GFXFF);
        tft.setFreeFont(FSS24);
        tft.drawString(lbi.frequency, 20 + i * 100, 130, GFXFF);
    }
    return true;
}

bool UI::Network_LoRa_Confirm(uint8_t select) {
    Widget_Title(2);
    cfg.lora_frequency = lora_band_info[select].band;
    tft.setTextColor(TFT_WHITE);
    tft.drawString("Please download and register an account", 25, 86, 2);
    tft.drawString("on our SenseCAP Mate APP, then scan the", 25, 100, 2);
    tft.drawString("QR code on the back of Grove-Wio E5", 25, 114, 2);
    tft.drawString("(which is included in the kit) to bind", 25, 128, 2);
    tft.drawString("your device to the cloud.", 25, 142, 2);

    Label_CentreBtn("Continue", 0x03ff);
    Label_Network();
    return true;
}

bool UI::Network_Disconnect(uint8_t select) {
    tft.fillRect(80, 80, 160, 80, TFT_WHITE);
    tft.setFreeFont(FSS9);
    tft.setTextColor(TFT_BLACK);
    tft.drawString("Disconnect?", 106, 90, GFXFF);
    if (select == 0) {
        tft.fillRect(104, 122, 40, 24, TFT_GREEN);
    } else {
        tft.fillRect(174, 122, 40, 24, TFT_GREEN);
    }

    tft.fillRect(106, 124, 36, 20, TFT_RED);
    tft.drawString("Yes", 110, 126, 2);

    tft.fillRect(176, 124, 36, 20, 0x02ff);
    tft.drawString("No", 186, 126, 2);
    return true;
}

void UI::ProcessPageManager(uint8_t key) {
    switch (key) {
    case LEFT_PRESSED:
        p_state.s_select--;
        if (p_state.s_select < 0) {
            p_state.s_select = 0;
        }
        break;
    case RIGHT_PRESSED:
        p_state.s_select++;
        // 只有两个选择
        if (p_state.s_select > 2) {
            p_state.s_select = 2;
        }
        break;
    case UP_PRESSED:
        p_state.current_page--;
        if (p_state.current_page < 0) {
            p_state.current_page = 0;
        }
        break;
    case SELECT_PRESSED:
        if (p_state.is_next) {
            p_state.current_page++;
            if (p_state.current_page > countof(process) - 1) {
                p_state.current_page = countof(process) - 1;
            }
        }
        break;
    }
    p_state.is_next = (this->*process[p_state.current_page])(p_state.s_select);
}

bool UI::Process_1(uint8_t select) {
    if (!layout_refresh) //this page not included data refresh
        return true;
    Widget_Title(1);
    Label_Network();
    switch (select) {
    case 0: // Vision AI real-time analysis
        Label_Subtitle("Vision AI real-time analysis");
        tft.fillRect(57, 100, 75, 60, tft.color565(0, 139, 0)); // Green
        break;
    case 1: // TinyML Example
        Label_Subtitle("TinyML Example");
        tft.fillRect(183, 100, 75, 40, tft.color565(0, 139, 0));
        break;
    // toDo: Data Filter
    }
    tft.setFreeFont(FSS9);
    tft.setTextColor(TFT_WHITE);
    tft.drawString("Vision AI", 59, 100, GFXFF);
    tft.drawString("real-time", 59, 122, GFXFF);
    tft.drawString("analysis", 59, 144, GFXFF);

    tft.drawString("TinyML", 185, 100, GFXFF);
    tft.drawString("Example", 185, 125, GFXFF);

    // layout_refresh = false;
    return true;
}

bool UI::Process_2(uint8_t select) {
    if (layout_refresh) {
        Widget_Title(1);
        Label_Network();
    }
    else if (select == 1) {
        return true;
    }
    switch (select) {
        case 0: // Vision AI real-time analysis
        {
            if (layout_refresh) {
                Widget_Title(1);
                Label_Network();
                Label_Subtitle("Vision AI real-time analysis");
            }
            int  i = 0;
            char buf[16];
            log_ready = false;
            if (a_log.size() == 0) {
                log_ready = true;
                break;
            }
            tft.setFreeFont(FSS9);
            for (auto data : a_log) {
                sprintf(buf, "[%02d:%02d:%02d]:", data.time / 1000 / 60 / 60,
                        data.time / 1000 / 60 % 60, data.time / 1000 % 60);
                tft.setTextColor(TFT_GREEN, TFT_BLACK);
                tft.drawString(buf, 20, 80 + i * 14, 2);
                tft.setTextColor(TFT_WHITE, TFT_BLACK);
                tft.setTextPadding(200);
                tft.drawString(data.data, 100, 80 + i * 14, 2);
                tft.setTextPadding(0);
                i++;
            }
            log_ready = true;
            break;
        }
        case 1: // TinyML Example
        {
            if (!layout_refresh) { //this page only included layout refresh
                break;
            }
            Widget_Title(1);
            Label_Network();
            Label_Subtitle("TinyML Example");
            double PIXELL = 3;
            tft.setTextColor(TFT_WHITE);
            tft.fillRect(35, 80, 113, 113, TFT_WHITE);
            QRCode   qrcode;
            uint8_t *qrcodeData = (uint8_t *)malloc(qrcode_getBufferSize(5)); // 1376/8
            qrcode_initText(&qrcode, qrcodeData, 5, 0,
                            "https://wiki.seeedstudio.com/K1100-Getting-Started/#tinyml-section");
            for (uint8_t y = 0; y < qrcode.size; y++) {// Each horizontal module
                for (uint8_t x = 0; x < qrcode.size; x++) {
                    if (qrcode_getModule(&qrcode, x, y))
                        tft.fillRect(x * PIXELL + 35 + 1, y * PIXELL + 80 + 1, PIXELL, PIXELL, TFT_BLACK);
                }
            }
            free(qrcodeData);
            tft.setFreeFont(FSS9);
            tft.drawString("Scan the QR ", 160, 106, GFXFF);
            tft.drawString("code to view ", 160, 126, GFXFF);
            tft.drawString("the tutorial ", 160, 146, GFXFF);
            break;
        }
    // toDo: Data Filter
    }
    return true;
}

void UI::SensePageManager(uint8_t key) {
    uint8_t s_num;
    switch (key) {
    case LEFT_PRESSED:
        s_state.s_select -= (s_state.s_select > 0) ? 1 : 0;
        break;
    case RIGHT_PRESSED:
        if (s_state.current_page == 0) {
            s_num = SENSOR_NUM_MAX;
        }//display sensor slots for maximum number
        else { 
            s_num = (s_data.size() < SENSOR_NUM_MAX) ? 
                    s_data.size() : SENSOR_NUM_MAX;
        }//display information about the connected sensors
        s_state.s_select += (s_state.s_select < s_num - 1) ? 1 : 0;
        break;
    case UP_PRESSED:
        s_state.current_page -= (s_state.current_page > 0) ? 1 : 0;
        break;
    case SELECT_PRESSED:
        if (s_state.is_next) { // current select is a sensor?
            if (s_state.current_page == 2) { // save sensor data to SD card
                if (cfg.sensor_save_flag & 1 << s_data[s_state.s_select].id) {
                    cfg.sensor_save_flag &= ~(1 << s_data[s_state.s_select].id);
                } else {
                    cfg.sensor_save_flag |= 1 << s_data[s_state.s_select].id;
                }
            } else {
                s_state.current_page ++;
            }
        }
        break;
    }

    s_state.is_next = (this->*sense[s_state.current_page])(s_state.s_select);
}

bool UI::Sensor_1(uint8_t select) {
    uint16_t       bg_color;
    static uint8_t index  = 0; // 指示页面最左侧传感器的序号
    s_data_ready = false;
    if (layout_refresh) 
    {
        Widget_Title(0);   //上方页面栏指示所在功能页
        Label_Network(); //左下角显示连接状态
        Label_Hardware((select<s_data.size())?0xff:0x00); //右下角提示grove接入
        Widget_PagePos(SENSOR_NUM_MAX/3+(SENSOR_NUM_MAX%3>0), select/3);
        //保证所选传感器在显示范围内，即 index <= select < index+3
        if (select >= index + 3) //所选传感器在当前页之后
            index += 1;
        else if (select < index) //所选传感器在当前页之前
            index = select;
    }
    if (s_data.size() == 0) { // 传感器还未准备好
        s_data_ready = true;
        tft.setTextColor(TFT_WHITE);
        tft.drawString("Loading...", 100, 110, 4);
        return false;
    }
    for (uint8_t si = 0; si < 3; si++) { // 显示序号为 index，index+1，index+2的传感器
        if (index+si == select) {
            bg_color = tft.color565(0, 139, 0);
            if (layout_refresh)
                tft.fillRect(20+si*100, 60, 90, 130, bg_color);
        } else {
            bg_color = TFT_BLACK;
        }
        if (index+si < s_data.size()) {
            Label_SensorInfo(s_data[index+si].name, s_data[index+si].data_unit, si);
            Label_SensorData(s_data[index+si], si, bg_color);
        } else {
            Label_SensorAdd(si);
        }
    }
    s_data_ready = true;
    return (select < s_data.size()) ? true : false;
}

bool UI::Sensor_2(uint8_t select) {
    static uint8_t select_old = 0;
    uint16_t       line_col[] = {TFT_GREEN, TFT_RED, TFT_BLUE, TFT_YELLOW};
    uint8_t        data_num   = 0;

    if (select > s_data.size() - 1)
        select = s_data.size() - 1;
    if (layout_refresh) {
        Widget_Title(0);
        Label_Subtitle(s_data[select].name);// Display the sensor name
        Widget_PagePos((SENSOR_NUM_MAX+2)/3, select / 3);
        Label_Network();
    }
    tft.fillRect(18, 78, 24, 90, TFT_WHITE);
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
        if (s_data[select].id == GROVE_SHT4X)
            line_chart_data[i].push(((int32_t *)s_data[select].data)[i]/100);
        else
            line_chart_data[i].push(((int32_t *)s_data[select].data)[i]);

        content.height(85)
            .width(260)
            .based_on(0.0)             // Starting point of y-axis, must be a float
            .show_circle(false)        // drawing a cirle at each point, default is on.
            .value(line_chart_data[i]) // passing through the data to line graph
            .max_size(LINE_DATA_MAX_SIZE)
            .color(line_col[i]) // Setting the color for the line
            // .backgroud(tft.color565(0,0,0)) // Setting the color for the backgroud
            .backgroud(tft.color565(0, 0, 0))
            .draw(&tft);
    }

    spr.setFreeFont(FSS9);
    spr.setTextColor(TFT_WHITE);
    spr.drawString("ADD", 14, 27, 4);

    spr.fillRect(20, 22 + 1.5 * FONT_ROW_HEIGHT, 40, 40, TFT_WHITE);
    spr.fillRect(38, 22 + 1.5 * FONT_ROW_HEIGHT + 6, 3, 26, TFT_BLACK);
    spr.fillRect(27, 22 + 1.5 * FONT_ROW_HEIGHT + 18, 26, 3, TFT_BLACK);

    // spr.pushSprite(220, 90);
    // spr.deleteSprite();
}

void UI::SensorPageState(int pages_num, int page_select) {

    spr.createSprite(340, 10);
    // 问题
    int *page_location = new int[pages_num];

    int temp = 0;

    for (int i = 0; i < pages_num; i++) {
        page_location[temp] = 140 - pages_num / 2 * 10 * (1 + pages_num % 2) + i * 20;
        temp++;
    }

    for (int i = 0; i < pages_num; i++) {
        if (i == page_select)
            tft.fillCircle(posX + 20 * i, 206, 3, tft.color565(0, 193, 255));
        else
            tft.fillCircle(posX + 20 * i, 206, 3, tft.color565(220, 220, 220));
    }
}

/* Display a four-frame icon to show network signal */
void UI::Widget_Signal(int16_t signal, int32_t x, int32_t y) {
    uint16_t color[4];
    if (signal > -70 && signal < 0) {
        color[0] = tft.color565(0, 139, 0);// Four-frame signal
        color[1] = tft.color565(0, 139, 0);
        color[2] = tft.color565(0, 139, 0);
        color[3] = tft.color565(0, 139, 0);
    } else if (signal > -90 && signal < -70) {
        color[0] = tft.color565(0, 139, 0);// Three-frame signal
        color[1] = tft.color565(0, 139, 0);
        color[2] = tft.color565(0, 139, 0);
        color[3] = tft.color565(100, 100, 100);
    } else if (signal > -110 && signal < -90) {
        color[0] = tft.color565(0, 139, 0);// Two-frame signal
        color[1] = tft.color565(0, 139, 0);
        color[2] = tft.color565(100, 100, 100);
        color[3] = tft.color565(100, 100, 100);
    } else if (signal > -130 && signal < -110) {
        color[0] = tft.color565(0, 139, 0);// One frame signal
        color[1] = tft.color565(100, 100, 100);
        color[2] = tft.color565(100, 100, 100);
        color[3] = tft.color565(100, 100, 100);
    } else {
        Status2Display(0);
        ret = false;
    }
    //获取显示数据的位置
    if (select >= index + 3) {
        index += 1;
    }
    if (select < index) {
        index = select;
    }
    //只显示select-1, select和select+1的数据
    //处理好特殊情况，第一个和最后一个
    for (int si = 0; si < 3; si++) {
        spr.createSprite(90, 130);
        //高亮选择的数据
        if (index + si == select) {
            spr.fillRect(0, 25, 90, 130, tft.color565(0, 139, 0));
        }

        if (index + si >= s_data.size()) {
            SensorADDDisplay(index + si == select);
        } else {
            // Display the sensor name
            SensorSubTitle(s_data[index + si].name);
            SensorUnit(s_data[index + si].data_unit);
            //显示数据： 0正常显示 1显示平均值
            spr.setFreeFont(FSS9);
            spr.setTextColor(TFT_WHITE);
            if (s_data[index + si].ui_type == SENSOR_UI_TYPE_NORMAL) {
                //一次只显示4个数据，每个测量数据4个byte
                if (s_data[index + si].size > 4 * 4)
                    sense_display_num = 4 * 4;
                else
                    sense_display_num = s_data[index + si].size;
                //把框分成4行，每行显示一个数据
                for (int i = 0; i < sense_display_num; i += 4) {
                    int32_t dd = ((uint8_t *)s_data[index + si].data)[i] << 0 |
                                 ((uint8_t *)s_data[index + si].data)[i + 1] << 8 |
                                 ((uint8_t *)s_data[index + si].data)[i + 2] << 16 |
                                 ((uint8_t *)s_data[index + si].data)[i + 3] << 24;
                    if (s_data[index + si].data_type == SENSOR_DATA_TYPE_INT32)
                        spr.drawString(String(dd), 2, 30 + 24 * i / 4,
                                       2 * (4 - sense_display_num / 4));
                    else if (s_data[index + si].data_type == SENSOR_DATA_TYPE_FLOAT)
                        spr.drawFloat((float)dd / 100, 2, 2, 30 + 24 * i / 4,
                                      2 * (4 - sense_display_num / 4));
                    // todo，数据单位，暂时显示为空
                    spr.drawString("  ", 68, 30 + 24 * i, 2);
                }
            } else {
                int32_t temp = 0;
                for (int i = 0; i < s_data[index + si].size; i += 4) {
                    temp += *(int32_t *)(s_data[index + si].data + i);
                }
                temp /= s_data[index + si].size / 4;
                spr.drawString(String(s_data[index + si].size / 4), 2, 30, 4);
                if (s_data[index + si].data_type == SENSOR_DATA_TYPE_INT32)
                    spr.drawString(String(temp), 2, 30 + 24, 4);
                else if (s_data[index + si].data_type == SENSOR_DATA_TYPE_FLOAT)
                    spr.drawFloat((float)temp / 100, 2, 2, 30 + 24, 4);
            }
        }
        //根据数据的index，显示不同的位置，一个页面只能显示三个，页面不够补+号。
        spr.pushSprite(20 + si * 100, 60);
        spr.deleteSprite();
    }
    // SensorADDDisplay(1);
    SensorPageState(s_data.size() / 3 + 1, select / 3);
    s_data_ready = true;
    Status1Display(0);
    // 暂时不支持折线图
    return false;
}

void UI::Label_SensorAdd(uint8_t pos) {
    if (!layout_refresh)
        return;
    tft.setFreeFont(FSS9);
    tft.setTextColor(TFT_WHITE);
    tft.drawString("ADD", 34+pos*100, 87, 4);
    tft.fillRect(40+pos*100, 82+1.5*FONT_ROW_HEIGHT, 40, 40, TFT_WHITE);
    tft.fillRect(58+pos*100, 82+1.5*FONT_ROW_HEIGHT+6, 3, 26, TFT_BLACK);
    tft.fillRect(47+pos*100, 82+1.5*FONT_ROW_HEIGHT+18, 26, 3, TFT_BLACK);
}

void UI::Label_SensorData(sensor_data& data, uint8_t pos, uint16_t bg_color) {
    uint8_t data_num = 0;
    tft.setFreeFont(FSS9);
    tft.setTextColor(TFT_WHITE, bg_color);
    tft.setTextDatum(TC_DATUM);
    tft.setTextPadding(90);
    if (data.ui_type == SENSOR_UI_TYPE_NORMAL) {
        //传感器需要同时显示的数据数量，最多4个，每个 4 bytes
        data_num = (data.size > 4*4) ? 4 : data.size/4;
        for (int i = 0; i < data_num; i ++) { 
            // int32_t dd =((uint8_t *)data.data)[4*i+0] << 0 |
            //             ((uint8_t *)data.data)[4*i+1] << 8 |
            //             ((uint8_t *)data.data)[4*i+2] << 16|
            //             ((uint8_t *)data.data)[4*i+3] << 24;
            int32_t dd = ((int32_t *)data.data)[i];
            uint8_t font =  (data_num == 3) ? FONT2 : 
                            (data_num == 2) ? FONT4 : 
                            (dd > 999)      ? FONT4 : FONT6;
            if (data.data_type == SENSOR_DATA_TYPE_INT32)
                tft.drawString(String(dd), 65+pos*100, 90+24*i, font);
            else if (data.data_type == SENSOR_DATA_TYPE_FLOAT)
                tft.drawFloat((float)dd/100, 2, 65+pos*100, 90+24*i, font);
            // tft.setTextPadding(width);
        }
    } else {
        int32_t temp = 0;
        for (int i = 0; i < data.size; i += 4) {
            temp += *(int32_t *)(data.data + i);
        }
        temp /= data.size / 4;
        tft.drawString(String(data.size/4), 65+pos*100, 90, 4);
        if (data.data_type == SENSOR_DATA_TYPE_INT32)
            tft.drawString(String(temp), 65+pos*100, 90+24, 4);
        else if (data.data_type == SENSOR_DATA_TYPE_FLOAT)
            tft.drawFloat((float)temp/100, 2, 65+pos*100, 90+24, 4);
    }
    tft.setTextDatum(TL_DATUM);
    tft.setTextPadding(0);
}

// display sensor chart subtitle 
void UI::Label_Subtitle(String value) {
    int16_t width;
    // tft.setFreeFont(FSSB9);
    tft.setFreeFont(FSS9);
    tft.setTextColor(TFT_WHITE, tft.color565(100, 100, 100));
    tft.setTextDatum(CC_DATUM);
    width = tft.textWidth(value) + 30;
    tft.fillRect((SCREEN_WIDTH-width)/2, 50, width, 26, tft.color565(100, 100, 100));
    tft.drawString(value, SCREEN_WIDTH/2, 50 + 26 / 2, GFXFF);
    tft.setTextDatum(TL_DATUM);
    // if (value.length() > 7) {
    //     tft.drawCentreString(value, SCREEN_WIDTH/2, 50, FONT2);
    // } else {
    //     tft.drawCentreString(value, SCREEN_WIDTH/2, 50, GFXFF);
    // }
}