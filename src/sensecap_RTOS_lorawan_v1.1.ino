#include <TFT_eSPI.h>
#include <Wire.h>
#include <Seeed_Arduino_FreeRTOS.h>
#include "disk91_LoRaE5.h"
#include "LIS3DHTR.h"
#include "Seeed_Arduino_GroveAI.h"

TFT_eSPI tft = TFT_eSPI();
TFT_eSprite spr = TFT_eSprite(&tft); // Sprite
LIS3DHTR<TwoWire> lis;

// 显示尺寸设置
#define SCREEN_WIDTH 320
#define SCREEN_HIGH 240
#define SCREEN_PADDING 10
#define PIXEL 4
const static unsigned int FONT_SIZE = 2;        // rate
const static unsigned int FONT_ROW_HEIGHT = 22; // px
const static unsigned int FONT_LEFT_START = 0;  // px
const static unsigned int FONT_COLOR = TFT_WHITE;
const static unsigned int FONT_BG_COLOR = TFT_RED;
const static unsigned int BACKGROUD = TFT_BLACK;

// Vision AI
GroveAI ai(Wire);
int conf_avg = 0;

// Grove LoRa E5
Disk91_LoRaE5 lorae5(&Serial); // Where the AT command and debut traces are printed
static int statue;

//全局变量
int conf = 0;                                     //置信度
int num = 0;                                      //人体数量
int light = 0, mic_val = 0;                       //光线值、麦克风响度
bool col = false;                                 //控制LoRaWAN灯色
float x_values = 0.0, y_value = 0.0, z_val = 0.0; // IMU数值
uint8_t state = 0;                                // Vision AI工作标志

// RTOS
TaskHandle_t Handle_pageTask;
TaskHandle_t Handle_monitorTask;
TaskHandle_t Handle_ReadSensor_Task;
TaskHandle_t Handle_VisionAI_Task;
TaskHandle_t Handle_JoinLora_Task;

// SenseCAP
//数据帧含义：首位1byte通道0x00设置信息通道，2byte数据内容编号（版本信息），4byte版本号，2byte CRC16
static unsigned char Version_data[9] = {0x00, 0x00, 0x00, 0x01, 0x01, 0x02, 0x02, 0xC5, 0x56};
//数据帧含义：首位1byte通道0x00设置信息通道，2byte数据内容编号（电量和上报周期），4byte电量100周期五分钟，2byte CRC16
static unsigned char PowerFrequency_data[9] = {0x00, 0x07, 0x00, 0x64, 0x00, 0x05, 0x00, 0x61, 0x89}; // 5min

//数据帧含义：首位1byte通道0x01发送数据，1byte 0x09表示VisionAI设置
// 1byte固定0x00，1byte表示功能选项：0x00目标检测；0x01表示目标计数
// 1byte表示模型类别：0x01表示人，0x02表示熊猫
// 2byte固定0x00，2byte CRC16
static unsigned char VisionAI_set_num[9] = {0x01, 0x09, 0x00, 0x01, 0x01, 0x00, 0x00, 0xC1, 0xFC};
static unsigned char VisionAI_set_conf[9] = {0x01, 0x09, 0x00, 0x00, 0x01, 0x00, 0x00, 0x7A, 0xE0};

//数据帧含义：首位1byte通道0x01发送数据，2byte设备ID编号，4byte数据内容（务必*1000），2byte CRC16
//光线4099、三轴4150、4151、4152、噪音4148
// VisionAI 4175,4176,4177,4178,4179,4180,4181,4182,4183,4184 每个识别的对象单独占一个通道
unsigned char Light_data[9] = {0x01, 0x61, 0x10, 0x00, 0x00, 0x00, 0x00};
unsigned char Sound_data[9] = {0x01, 0x60, 0x10, 0x00, 0x00, 0x00, 0x00};
unsigned char IMUx_data[9] = {0x01, 0x36, 0x10, 0x00, 0x00, 0x00, 0x00};
unsigned char IMUy_data[9] = {0x01, 0x37, 0x10, 0x00, 0x00, 0x00, 0x00};
unsigned char IMUz_data[9] = {0x01, 0x38, 0x10, 0x00, 0x00, 0x00, 0x00};
unsigned char VisionAI_data1[11] = {0x20, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}; // num
unsigned char VisionAI_data2[11] = {0x21, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
unsigned char VisionAI_data3[11] = {0x20, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}; // confidence
unsigned short int crc_data = 0x0000;

//按键
int key_status = 0;

// RTOS Delay
void myDelayMs(int ms)
{
    vTaskDelay((ms * 1000) / portTICK_PERIOD_US);
}

// RTOS：显示屏任务
static void LCD_TASK_1(void *pvParameters)
{
    Serial.println("Thread A: Started");
    while (1)
    {
        char str_num[1], str_conf[1], str_light[1], str_mic_val[1], str_x_values[8], str_y_value[8], str_z_val[8];
        sprintf(str_num, "%d", num);
        sprintf(str_conf, "%d", conf_avg);
        sprintf(str_light, "%d", light);
        sprintf(str_mic_val, "%d", mic_val);
        snprintf((char *)str_x_values, 8, "%.2f", x_values);
        snprintf((char *)str_y_value, 8, "%.2f", y_value);
        snprintf((char *)str_z_val, 8, "%.2f", z_val);

        conf_avg = 0;

        // Serial.println(str_mic_val);

        spr.createSprite(300, 230);
        spr.setTextSize(1); //设置字体大小

        spr.fillRect(0, 32, 320, 28, tft.color565(61, 145, 64));
        // spr.fillRect(0, 60, 320, 100, tft.color565(220, 220, 220));
        spr.fillRect(0, 60, 320, 100, tft.color565(163, 148, 128));

        spr.setTextColor(TFT_WHITE); //设置字体颜色和背景颜色
        // spr.fillRect(0, 0, 290, 30, TFT_GREEN);
        spr.drawString("                              LoRaWAN", FONT_LEFT_START, 5, 4);
        // spr.drawString("                              LoRaWAN", FONT_LEFT_START, 2, 2);

        // spr.setTextColor(tft.color565(61, 89, 171), TFT_BLACK);
        // spr.setTextColor(tft.color565(250, 128, 114), TFT_BLACK);
        // spr.drawString("  Vision AI Module     On-board Sensor", FONT_LEFT_START, 32, 2); //写字（参数：字符串、开始x坐标、开始y坐标、字体）
        spr.setTextColor(TFT_BLACK); //设置字体颜色和背景颜色
        // tft.setFreeFont(FF10);
        spr.drawString("  Vision AI Module     On-board Sensor", FONT_LEFT_START, 36, 2); //写字（参数：字符串、开始x坐标、开始y坐标、字体）
        spr.drawString("  People Counting: ", FONT_LEFT_START, 32 + 1.5 * FONT_ROW_HEIGHT, 2);
        spr.drawString("  Average", FONT_LEFT_START, 32 + 2.5 * FONT_ROW_HEIGHT, 2);
        spr.drawString("  Accuracy:", FONT_LEFT_START, 32 + 3.5 * FONT_ROW_HEIGHT, 2);
        spr.drawString("%", FONT_LEFT_START + 100, 32 + 3.5 * FONT_ROW_HEIGHT, 2);
        spr.drawString("Light: ", FONT_LEFT_START + 149, 32 + 1.5 * FONT_ROW_HEIGHT, 2);
        spr.drawString("Sound: ", FONT_LEFT_START + 149, 32 + 2.5 * FONT_ROW_HEIGHT, 2);
        spr.drawString("IMU: ", FONT_LEFT_START + 149, 32 + 3.5 * FONT_ROW_HEIGHT, 2);
        spr.drawString("(x,y,z)", FONT_LEFT_START + 149, 32 + 4.5 * FONT_ROW_HEIGHT, 2);
        // spr.drawLine(0, 32+5.5*FONT_ROW_HEIGHT, 300, 32+5.5*FONT_ROW_HEIGHT, TFT_BLACK);
        spr.drawLine(FONT_LEFT_START + 140, 32, FONT_LEFT_START + 140, 160, TFT_BLACK);

        spr.setTextColor(TFT_WHITE); //设置字体颜色和背景颜色
        if (key_status == 1)
        {
            spr.drawString("  EU868", FONT_LEFT_START, 5, 2);
        }
        else if (key_status == 2)
        {
            spr.drawString("  US915", FONT_LEFT_START, 5, 2);
        }
        else if (key_status == 3)
        {
            spr.drawString("  AU915", FONT_LEFT_START, 5, 2);
        }

        if (col)
        {
            spr.fillCircle(FONT_LEFT_START + 280, 14, 10, TFT_GREEN);
        }
        else
        {
            spr.fillCircle(FONT_LEFT_START + 280, 14, 10, TFT_RED);
        }

        // spr.drawRect(FONT_LEFT_START+23, 25+6*FONT_ROW_HEIGHT, 248, 55, TFT_BLUE);

        spr.setTextColor(tft.color565(25, 25, 112), tft.color565(163, 148, 128)); //设置字体颜色和背景颜色
        spr.drawString(str_num, FONT_LEFT_START + 123, 32 + 1.5 * FONT_ROW_HEIGHT, 2);
        spr.drawString(str_conf, FONT_LEFT_START + 80, 32 + 3.5 * FONT_ROW_HEIGHT, 2);
        spr.drawString(str_light, FONT_LEFT_START + 186, 32 + 1.5 * FONT_ROW_HEIGHT, 2);
        spr.drawString(str_mic_val, FONT_LEFT_START + 194, 32 + 2.5 * FONT_ROW_HEIGHT, 2);
        spr.drawString(str_x_values, FONT_LEFT_START + 179, 32 + 3.5 * FONT_ROW_HEIGHT, 2);
        spr.drawString(str_y_value, FONT_LEFT_START + 214, 32 + 3.5 * FONT_ROW_HEIGHT, 2);
        spr.drawString(str_z_val, FONT_LEFT_START + 254, 32 + 3.5 * FONT_ROW_HEIGHT, 2);

        spr.setTextColor(TFT_BLACK); //设置字体颜色和背景颜色
        spr.drawString(",", FONT_LEFT_START + 210, 32 + 3.5 * FONT_ROW_HEIGHT, 2);
        spr.drawString(",", FONT_LEFT_START + 250, 32 + 3.5 * FONT_ROW_HEIGHT, 2);

        spr.setTextColor(TFT_WHITE); //设置字体颜色和背景颜色
        spr.drawString("Please login to                    to", FONT_LEFT_START + 30, 32 + 6.5 * FONT_ROW_HEIGHT, 2);
        spr.drawString("bind your device.", FONT_LEFT_START + 30, 32 + 7.5 * FONT_ROW_HEIGHT, 2);

        spr.setTextColor(tft.color565(127, 255, 0)); //设置字体颜色和背景颜色
        spr.drawString("sensecap.seeed.cc", FONT_LEFT_START + 129, 32 + 6.5 * FONT_ROW_HEIGHT, 2);

        spr.pushSprite(0 + SCREEN_PADDING, 0 + SCREEN_PADDING);
        spr.deleteSprite();
        myDelayMs(200);
    }
}

void PAGE() //设置频段页面
{
    tft.fillScreen(TFT_BLACK);
    tft.fillRect(0, 140, 320, 100, TFT_WHITE);
    tft.setTextSize(1);
    tft.setTextColor(TFT_BLACK);
    tft.setTextDatum(MC_DATUM);
    tft.fillRect(0, 0, 30, 10, TFT_CYAN);
    tft.fillRect(73, 0, 40, 10, TFT_GREEN);
    tft.fillRect(154, 0, 40, 10, TFT_YELLOW);
    tft.drawString("Please select LoRaWAN", 160, 160, 4);
    tft.drawString("frequency bands for your", 160, 190, 4);
    tft.drawString("device connection.", 160, 220, 4);

    tft.drawLine(12, 30, 12, 125, TFT_CYAN);
    tft.drawLine(12, 125, 245, 125, TFT_CYAN);

    tft.drawLine(90, 30, 90, 105, TFT_GREEN);
    tft.drawLine(90, 105, 245, 105, TFT_GREEN);

    tft.drawLine(168, 30, 168, 85, TFT_YELLOW);
    tft.drawLine(168, 85, 245, 85, TFT_YELLOW);

    // tft.setTextSize(2);
    tft.setTextColor(TFT_CYAN, TFT_BLACK);
    tft.drawString("Button A", 0, 23, 2);
    tft.drawString("EU868", 280, 125, 2);

    tft.setTextColor(TFT_GREEN, TFT_BLACK);
    tft.drawString("Button B", 95, 23, 2);
    tft.drawString("US915", 280, 105, 2);

    tft.setTextColor(TFT_YELLOW, TFT_BLACK);
    tft.drawString("Button C", 174, 23, 2);
    tft.drawString("AU915", 280, 85, 2);

    tft.setTextDatum(0);
}

void keyInit() //设置按键
{
    pinMode(WIO_KEY_C, INPUT_PULLUP);
    pinMode(WIO_KEY_B, INPUT_PULLUP);
    pinMode(WIO_KEY_A, INPUT_PULLUP);
    while (1)
    {
        if (digitalRead(WIO_KEY_C) == LOW)
        {
            Serial.println("A Key pressed");
            key_status = 1;
            break;
        }
        else if (digitalRead(WIO_KEY_B) == LOW)
        {
            Serial.println("B Key pressed");
            key_status = 2;
            break;
        }
        else if (digitalRead(WIO_KEY_A) == LOW)
        {
            Serial.println("C Key pressed");
            key_status = 3;
            break;
        }
    }
}

// RTOS: 内置传感器读值任务
static void read_sensor(void *pvParameters)
{
    Serial.println("Thread B: Started");
    while (1)
    {
        light = analogRead(WIO_LIGHT);
        mic_val = analogRead(WIO_MIC);
        // Serial.println(mic_val);
        lis.getAcceleration(&x_values, &y_value, &z_val);
        data_decord(light, Light_data, 1);
        data_decord(mic_val, Sound_data, 1);
        data_decord(x_values, IMUx_data, 1);
        data_decord(y_value, IMUy_data, 1);
        data_decord(z_val, IMUz_data, 1);
        crc_data = GetCrc16(Light_data, 7, Light_data);
        crc_data = GetCrc16(Sound_data, 7, Sound_data);
        crc_data = GetCrc16(IMUx_data, 7, IMUx_data);
        crc_data = GetCrc16(IMUy_data, 7, IMUy_data);
        crc_data = GetCrc16(IMUz_data, 7, IMUz_data);
        myDelayMs(480);
    }
}

// Vision AI 初始化
void VisionAI_Init()
{
    if (ai.begin(ALGO_OBJECT_DETECTION, MODEL_EXT_INDEX_1)) // Object detection and pre-trained model 1
    {
        state = 1;
    }
    else
    {
        Serial.println("Algo begin failed.");
    }
}

// RTOS：VisionAI训练读值
static void VisionAI_Function(void *pvParameters)
{
    Serial.println("Thread C: Started");
    while (1)
    {
        for (int j = 1; j < 11; j++)
        {
            VisionAI_data2[j] = 0xFF;
            VisionAI_data3[j] = 0xFF;
        }
        if (ai.invoke()) // begin invoke
        {
            while (1) // wait for invoking finished
            {
                CMD_STATE_T ret = ai.state();
                if (ret == CMD_STATE_IDLE)
                {
                    break;
                }
                myDelayMs(20);
            }
            uint8_t len = ai.get_result_len(); // receive how many people detect
            if (len)
            {
                num = len;
                object_detection_t data; // get data
                VisionAI_data1[2] = len;
                for (int i = 0; i < len; i++)
                {
                    ai.get_result(i, (uint8_t *)&data, sizeof(object_detection_t)); // get result
                    conf = data.confidence;
                    conf_avg = conf_avg + conf;
                    if (i < 5)
                    {
                        VisionAI_data3[2 * i + 1] = 1;
                        VisionAI_data3[2 * i + 2] = conf;
                    }
                    else if (i > 4)
                    {
                        if (i > 9)
                            break;
                        VisionAI_data2[2 * i - 9] = 1;
                        VisionAI_data2[2 * i - 8] = conf;
                    }
                }
                conf_avg = conf_avg / len;
            }
            else
            {
                // Serial.println("No identification");
                num = 0;
                conf = 0.0;
                conf_avg = 0;
            }
        }
        else
        {
            Serial.println("Invoke Failed.");
        }
        myDelayMs(280);
    }
}

// lora e5初始化
void Lora_Init()
{
    static uint8_t Frequency;

    // init the library, search the LORAE5 over the different WIO port available
    if (!lorae5.begin(DSKLORAE5_SWSERIAL_WIO_P2))
    {
        Serial.println("LoRa E5 Init Failed");
        statue = 1;
    }
    else
    {
        statue = 2;
    }
    // Setup the LoRaWan Credentials
    if (statue == 2)
    {
        if (key_status == 1)
        {
            Frequency = {DSKLORAE5_ZONE_EU868};
        }
        else if (key_status == 2)
        {
            Frequency = {DSKLORAE5_ZONE_US915};
        }
        else if (key_status == 3)
        {
            Frequency = {DSKLORAE5_ZONE_AU915};
        }
        if (!lorae5.setup_sensecap(Frequency))
        { // Setup the LoRaWAN stack with the stored credentials
            Serial.println("LoRa E5 Setup Failed");
            statue = 1;
        }
    }
}

// lora e5连接LoRa并且发送数据
static void Join_sent_data(void *pvParameters)
{
    Serial.println("Thread E: Started");
    while (1)
    {
        myDelayMs(5000);
        if (lorae5.join_sync())
        {
            Serial.println("Success in the LoRa");
            col = true;
            myDelayMs(10000);
            static uint8_t data_1[9] = {0x00, 0x07, 0x00, 0x64, 0x00, 0x05, 0x00, 0x61, 0x89};
            if (lorae5.send_sync(2, data_1, sizeof(data_1), false, 7, 14))
            {
                Serial.println("Uplink Electricity information done");
                col = true;
            }
            else
                col = false;
            myDelayMs(10000);
            static uint8_t data_2[9] = {0x00, 0x00, 0x00, 0x01, 0x01, 0x02, 0x02, 0xC5, 0x56};
            if (lorae5.send_sync(2, data_2, sizeof(data_2), false, 7, 14))
            {
                Serial.println("Uplink Version information done");
                col = true;
            }
            else
                col = false;
            myDelayMs(10000);
            if (lorae5.send_sync(2, Light_data, sizeof(Light_data), false, 7, 14))
            {
                Serial.println("Uplink Light data done");
                col = true;
            }
            else
                col = false;
            myDelayMs(10000);
            if (lorae5.send_sync(2, Sound_data, sizeof(Sound_data), false, 7, 14))
            {
                Serial.println("Uplink Sound data done");
                col = true;
            }
            else
                col = false;
            myDelayMs(10000);
            if (lorae5.send_sync(2, IMUx_data, sizeof(IMUx_data), false, 7, 14))
            {
                Serial.println("Uplink IMU_x data done");
                col = true;
            }
            else
                col = false;
            myDelayMs(10000);
            if (lorae5.send_sync(2, IMUy_data, sizeof(IMUy_data), false, 7, 14))
            {
                Serial.println("Uplink IMU_y data done");
                col = true;
            }
            else
                col = false;
            myDelayMs(10000);
            if (lorae5.send_sync(2, IMUz_data, sizeof(IMUz_data), false, 7, 14))
            {
                Serial.println("Uplink IMU_z data done");
                col = true;
            }
            else
                col = false;
            myDelayMs(10000);
            static uint8_t data_3[9] = {0x01, 0x09, 0x00, 0x00, 0x01, 0x00, 0x00, 0x7A, 0xE0};
            if (lorae5.send_sync(2, data_3, sizeof(data_3), false, 7, 14))
            {
                Serial.println("Uplink confidence level ready");
                col = true;
            }
            else
                col = false;
            myDelayMs(10000);
            if (lorae5.send_sync(3, VisionAI_data3, sizeof(VisionAI_data3), false, 7, 14))
            {
                Serial.println("Uplink confidence_1 done");
                col = true;
            }
            else
                col = false;
            myDelayMs(10000);
            if (lorae5.send_sync(3, VisionAI_data2, sizeof(VisionAI_data2), false, 7, 14))
            {
                Serial.println("Uplink confidence_2 done");
                col = true;
            }
            else
                col = false;
            Serial.println("This transmission ends and the next round of data transmission begins in five minutes.");
            myDelayMs(300000);
        }
        else
            col = false;
    }
}

// RTOS: 需要栈空间统计
void taskMonitor(void *pvParameters)
{
    int x;
    int measurement;

    Serial.println("Task Monitor: Started");

    // run this task a few times before exiting forever
    for (x = 0; x < 2; ++x)
    {

        Serial.println("");
        Serial.println("******************************");
        Serial.println("[Stacks Free Bytes Remaining] ");

        measurement = uxTaskGetStackHighWaterMark(Handle_JoinLora_Task);
        Serial.print("Thread D: ");
        Serial.println(measurement);

        Serial.println("******************************");

        delay(10000); // print every 10 seconds
    }

    // delete ourselves.
    // Have to call this or the system crashes when you reach the end bracket and then get scheduled.
    Serial.println("Task Monitor: Deleting");
    vTaskDelete(NULL);
}

// CRC校验
static const uint16_t crc16tab[256] =
    {
        0x0000, 0x1189, 0x2312, 0x329b, 0x4624, 0x57ad, 0x6536, 0x74bf,
        0x8c48, 0x9dc1, 0xaf5a, 0xbed3, 0xca6c, 0xdbe5, 0xe97e, 0xf8f7,
        0x1081, 0x0108, 0x3393, 0x221a, 0x56a5, 0x472c, 0x75b7, 0x643e,
        0x9cc9, 0x8d40, 0xbfdb, 0xae52, 0xdaed, 0xcb64, 0xf9ff, 0xe876,
        0x2102, 0x308b, 0x0210, 0x1399, 0x6726, 0x76af, 0x4434, 0x55bd,
        0xad4a, 0xbcc3, 0x8e58, 0x9fd1, 0xeb6e, 0xfae7, 0xc87c, 0xd9f5,
        0x3183, 0x200a, 0x1291, 0x0318, 0x77a7, 0x662e, 0x54b5, 0x453c,
        0xbdcb, 0xac42, 0x9ed9, 0x8f50, 0xfbef, 0xea66, 0xd8fd, 0xc974,
        0x4204, 0x538d, 0x6116, 0x709f, 0x0420, 0x15a9, 0x2732, 0x36bb,
        0xce4c, 0xdfc5, 0xed5e, 0xfcd7, 0x8868, 0x99e1, 0xab7a, 0xbaf3,
        0x5285, 0x430c, 0x7197, 0x601e, 0x14a1, 0x0528, 0x37b3, 0x263a,
        0xdecd, 0xcf44, 0xfddf, 0xec56, 0x98e9, 0x8960, 0xbbfb, 0xaa72,
        0x6306, 0x728f, 0x4014, 0x519d, 0x2522, 0x34ab, 0x0630, 0x17b9,
        0xef4e, 0xfec7, 0xcc5c, 0xddd5, 0xa96a, 0xb8e3, 0x8a78, 0x9bf1,
        0x7387, 0x620e, 0x5095, 0x411c, 0x35a3, 0x242a, 0x16b1, 0x0738,
        0xffcf, 0xee46, 0xdcdd, 0xcd54, 0xb9eb, 0xa862, 0x9af9, 0x8b70,
        0x8408, 0x9581, 0xa71a, 0xb693, 0xc22c, 0xd3a5, 0xe13e, 0xf0b7,
        0x0840, 0x19c9, 0x2b52, 0x3adb, 0x4e64, 0x5fed, 0x6d76, 0x7cff,
        0x9489, 0x8500, 0xb79b, 0xa612, 0xd2ad, 0xc324, 0xf1bf, 0xe036,
        0x18c1, 0x0948, 0x3bd3, 0x2a5a, 0x5ee5, 0x4f6c, 0x7df7, 0x6c7e,
        0xa50a, 0xb483, 0x8618, 0x9791, 0xe32e, 0xf2a7, 0xc03c, 0xd1b5,
        0x2942, 0x38cb, 0x0a50, 0x1bd9, 0x6f66, 0x7eef, 0x4c74, 0x5dfd,
        0xb58b, 0xa402, 0x9699, 0x8710, 0xf3af, 0xe226, 0xd0bd, 0xc134,
        0x39c3, 0x284a, 0x1ad1, 0x0b58, 0x7fe7, 0x6e6e, 0x5cf5, 0x4d7c,
        0xc60c, 0xd785, 0xe51e, 0xf497, 0x8028, 0x91a1, 0xa33a, 0xb2b3,
        0x4a44, 0x5bcd, 0x6956, 0x78df, 0x0c60, 0x1de9, 0x2f72, 0x3efb,
        0xd68d, 0xc704, 0xf59f, 0xe416, 0x90a9, 0x8120, 0xb3bb, 0xa232,
        0x5ac5, 0x4b4c, 0x79d7, 0x685e, 0x1ce1, 0x0d68, 0x3ff3, 0x2e7a,
        0xe70e, 0xf687, 0xc41c, 0xd595, 0xa12a, 0xb0a3, 0x8238, 0x93b1,
        0x6b46, 0x7acf, 0x4854, 0x59dd, 0x2d62, 0x3ceb, 0x0e70, 0x1ff9,
        0xf78f, 0xe606, 0xd49d, 0xc514, 0xb1ab, 0xa022, 0x92b9, 0x8330,
        0x7bc7, 0x6a4e, 0x58d5, 0x495c, 0x3de3, 0x2c6a, 0x1ef1, 0x0f78};

uint16_t GetCrc16(uint8_t *buf, uint16_t len, unsigned char data[9])
{
    uint16_t crc = 0;

    for (uint16_t i = 0; i < len; i++)
        crc = (crc >> 8) ^ crc16tab[(crc ^ (*buf++)) & 0xFF];

    data[8] = crc >> 8 & 0xFF;
    data[7] = crc & 0xFF;

    return crc;
}

//传感器数据值处理
void data_decord(float val, unsigned char data[], int flag)
{
    int vals = val * 1000;
    // Serial.println(vals);
    if (val > 0 || val == 0)
    {
        if (flag == 1) //小端模式
        {
            data[6] = vals >> 24 & 0xFF;
            data[5] = vals >> 16 & 0xFF;
            data[4] = vals >> 8 & 0xFF;
            data[3] = vals & 0xFF;
        }
        else if (flag == 2) //大端模式
        {
            data[3] = vals >> 24 & 0xFF;
            data[4] = vals >> 16 & 0xFF;
            data[5] = vals >> 8 & 0xFF;
            data[6] = vals & 0xFF;
        }
    }
    else
    {
        vals = abs(vals);
        vals = ~vals + 1;
        if (flag == 1) //小端模式
        {
            data[6] = vals >> 24 | 0x80;
            data[5] = vals >> 16 & 0xFF;
            data[4] = vals >> 8 & 0xFF;
            data[3] = vals & 0xFF;
        }
        else if (flag == 2) //大端模式
        {
            data[3] = vals >> 24 | 0x80;
            data[4] = vals >> 16 & 0xFF;
            data[5] = vals >> 8 & 0xFF;
            data[6] = vals & 0xFF;
        }
    }
}

void setup()
{
    // put your setup code here, to run once:
    Serial.begin(9600);

    vNopDelayMS(1000); // prevents usb driver crash on startup, do not omit this
    // while (!Serial) ;

    tft.begin();
    tft.setRotation(3);
    tft.fillScreen(TFT_BLACK);

    Wire.begin();

    pinMode(WIO_LIGHT, INPUT);
    pinMode(WIO_MIC, INPUT);

    PAGE();
    keyInit();

    tft.fillScreen(TFT_BLACK);

    VisionAI_Init();

    lis.begin(Wire1);
    if (!lis)
    {
        Serial.println("IMU ERROR");
    }

    lis.setOutputDataRate(LIS3DHTR_DATARATE_25HZ); // Data output rate
    lis.setFullScaleRange(LIS3DHTR_RANGE_2G);

    Serial.println("");
    Serial.println("******************************");
    Serial.println("        Program start         ");
    Serial.println("******************************");

    Lora_Init();

    xTaskCreate(LCD_TASK_1, "Task A", 256, NULL, tskIDLE_PRIORITY + 1, &Handle_pageTask);
    xTaskCreate(read_sensor, "Task B", 128, NULL, tskIDLE_PRIORITY + 2, &Handle_ReadSensor_Task);
    if (state == 1)
    {
        xTaskCreate(VisionAI_Function, "Task C", 128, NULL, tskIDLE_PRIORITY + 2, &Handle_VisionAI_Task);
    }
    if (statue == 2)
    {
        xTaskCreate(Join_sent_data, "Task E", 1024, NULL, tskIDLE_PRIORITY + 3, &Handle_JoinLora_Task);
    }

    vTaskStartScheduler();
}

// RTOS空闲循环
void loop()
{
    // put your main code here, to run repeatedly:
    Serial.print("."); //在终端中打印点，我们仅在RTOS处于空闲状态时执行此操作
    delay(100);        //与vNopDelayMS不同，延迟对中断友好
}
