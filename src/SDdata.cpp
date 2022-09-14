#include "SDdata.h"

using namespace cpp_freertos;

SemaphoreHandle_t SDdata::lock = NULL;

SDdata::SDdata() {
}

SDdata::~SDdata() {
}

bool SDdata::init() {
    if (SDdata::lock == NULL) {
        SDdata::lock = xSemaphoreCreateMutex();
    }
    rtc.begin();
    DateTime now = DateTime(F(__DATE__), F(__TIME__));
    rtc.adjust(now);
    now = rtc.now();
    if (!SD.begin(SDCARD_SS_PIN, SDCARD_SPI, 4000000UL)) { // SD卡
        LOGSS.println("initialization failed!");
        is_connected = false;
    }
    is_connected = true;
    return is_connected;
}
// 0: success 1: disconnected 2: SD card 
uint8_t SDdata::status() {
    uint64_t cardSize;
    // xSemaphoreTake(SDdata::lock, portMAX_DELAY);
    if (!is_connected) {
        SD.end();
        if (!SD.begin(SDCARD_SS_PIN, SDCARD_SPI, 4000000UL)) { // SD卡
            LOGSS.println("initialization failed!");
            is_connected = false;
        return 1;
        }
    }
    cardSize = SD.totalBytes();
    if(cardSize == 0) {
        LOGSS.println("SD card is disconnected!");
        is_connected = false;
        return 1;
    }
    is_connected = true;
    if (cardSize - SD.usedBytes() < 1024 * 1024) {
        LOGSS.println("SD card is full!");
        return 2;
    }
    return 0;
    // xSemaphoreGive(SDdata::lock);
}
//保存传感器数值到SD卡，参数：传感器名（同时也是文件名称），数据，数据长度
template <typename T> void SDdata::saveData(String sensorName, T *sensorData, int len) {
    static int in = 0;
    xSemaphoreTake(SDdata::lock, portMAX_DELAY);
    String fileName = sensorName + ".csv";
    // LOGSS.println(fileName);
    if (SD.exists(fileName)) // header只写一次
    {
        myFile = SD.open(fileName, FILE_WRITE); //追加写模式
        LOGSS.println("file does not found, creating...");
        myFile.print(sensorName + ",");
        myFile.print("timestamp,");
        for (int i = 0; i < len - 1; i++) {
            myFile.print("data[" + String(i + 1) + "]");
            myFile.print(",");
        }
        myFile.println("data[" + String(len) + "]");
        myFile.close();
    }
    myFile = SD.open(fileName, FILE_APPEND); //追加写模式
    myFile.print(sensorName);
    myFile.print(",");

    DateTime now = rtc.now(); //读写当前时间
    myFile.print(String(now.year()) + "-" + String(now.month()) + "-" + String(now.day()) + " " +
                 String(now.hour()) + ":" + String(now.minute()) + ":" + String(now.second()) +
                 ",");

    for (int i = 0; i < len - 1; i++) //写入传感器值
    {
        myFile.print(sensorData[i]);
        // LOGSS.println(sensorData[i]);
        myFile.print(",");
    }
    myFile.println(sensorData[len - 1]); //最后一个传感器数值加换行
    // LOGSS.println(sensorData[len - 1]);

    myFile.close();
    xSemaphoreGive(SDdata::lock);
}
template void SDdata::saveData(String sensorName, int32_t *sensorData, int len);

void SDdata::Readdata(String sensorName) {
    xSemaphoreTake(SDdata::lock, portMAX_DELAY);
    CSV_Parser cp(/*format*/ "ssffff", /*has_header*/ true, /*delimiter*/ ',');

    String fileName = sensorName + ".csv";

    char arr[sensorName.length() + 1];
    strcpy(arr, sensorName.c_str());
    if (cp.readSDfile2(fileName)) { //读模式打开文件，读header
        char **column_1 = (char **)cp[arr];
        char **column_2 = (char **)cp["timestamp"];
        float *column_3 = (float *)cp["data1"];
        float *column_4 = (float *)cp["data2"];
        float *column_5 = (float *)cp["data3"];
        float *column_6 = (float *)cp["data4"];
        // float *column_7 = (float*)cp["data5"];

        for (int i = 0; i < cp.getRowsCount(); i++) { //解析csv文件
            LOGSS.print(column_1[i]);
            LOGSS.print(" - ");
            LOGSS.print(column_2[i]);
            LOGSS.print(" - ");
            LOGSS.print(column_3[i], DEC);
            LOGSS.print(" - ");
            LOGSS.print(column_4[i], DEC);
            LOGSS.print(" - ");
            LOGSS.print(column_5[i], DEC);
            LOGSS.print(" - ");
            LOGSS.print(column_6[i], DEC);
            LOGSS.println();
            // LOGSS.print(column_6[i], DEC);        LOGSS.print(" - ");
            // LOGSS.print(column_7[i], DEC);        LOGSS.println();
        }

        // output parsed values (allows to check that the file was parsed correctly)
        cp.print(); // assumes that "LOGSS.begin()" was called before (otherwise it won't work)
    } else {
        LOGSS.println("ERROR: File does not exist...");
    }
    xSemaphoreGive(SDdata::lock);
}