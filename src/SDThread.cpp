#include "SDThread.h"
#include "utils.h"

SDThread::SDThread(SysConfig &config) : Thread("SDThread", 128 * 22, 2), cfg(config) {
    Start();
}

// 0- not init, 1-  connected, 2- sd full
uint8_t SDThread::status() {
    uint64_t cardSize;
    if (cfg.sd_status == 0) {
        SD.end();
        if (!SD.begin(SDCARD_SS_PIN, SDCARD_SPI, 4000000UL)) { // SD卡
            LOGSS.println("initialization failed!");
            return 0;
        }
    }
    cardSize = SD.totalBytes();
    if (cardSize == 0) {
        LOGSS.println("SD card is disconnected!");
        return 0;
    }
    cfg.sd_status = 1;
    if (cardSize - SD.usedBytes() < 1024 * 1024) {
        LOGSS.println("SD card is full!");
        return 2;
    }
    return 1;
}
//保存传感器数值到SD卡，参数：传感器名（同时也是文件名称），数据，数据长度
void SDThread::saveData(String sensorName, int32_t *sensorData, int len, uint8_t type) {
    String fileName = sensorName + ".csv";
    // LOGSS.println(fileName);
    if (SD.exists(fileName)) // header只写一次
    {
        myFile = SD.open(fileName, FILE_WRITE); //追加写模式
        LOGSS.printf("file %s does not found, creating...\r\n", fileName.c_str());
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

    for (int i = 0; i < len; i++) //写入传感器值
    {
        /* 根据传感器值类型进行写入 */
        if (type == SENSOR_DATA_TYPE_FLOAT) {
            myFile.print(sensorData[i]/100);
            myFile.print(".");
            myFile.print(sensorData[i]%100);
        } else {
            myFile.print(sensorData[i]);
        }
        /* 判断是否写完所有传感器值 */
        if (i+1 < len) {
            myFile.print(",");
        } else {
            myFile.println(" ");
        }
    }

    myFile.close();
}

void SDThread::Readdata(String sensorName) {
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
}

void SDThread::Run() {
    if (wait_sd_data == NULL) {
        wait_sd_data = xSemaphoreCreateBinary();
    }
    rtc.begin();
    DateTime now = DateTime(F(__DATE__), F(__TIME__));
    rtc.adjust(now);
    now = rtc.now();
    while (true) {
        if (xSemaphoreTake(wait_sd_data, portMAX_DELAY) == pdTRUE) {
            sd_data_ready = false;
            for (auto data : sd_data) {
                if (cfg.sensor_save_flag & (1 << data.id)) {
                    cfg.sd_status = status();
                    if (cfg.sd_status == 1) {
                        saveData(String(data.name), (int32_t *)data.data, data.size/4, 
                                 data.data_type);
                    }
                }
            }
            sd_data_ready = true;
        }
        Delay(Ticks::MsToTicks(1000));
    }
}

void SDThread::SDPushData(std::vector<sensor_data *> d) {
    // A loop to deep copy param of d vector into new sd_data queue
    // by Iterative method
    if (sd_data_ready) {
        sd_data.clear();
        sd_data.shrink_to_fit();
        for (auto data : d)
            sd_data.push_back(*data);
        if (wait_sd_data != NULL) {
            xSemaphoreGive(wait_sd_data);
        }
    }
}