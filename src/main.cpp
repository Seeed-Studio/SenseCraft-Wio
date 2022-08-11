#include <CSV_Parser.h>
#include <SPI.h>
#include <Seeed_FS.h>
#include "Seeed_SD.h"
#include "RTC_SAMD51.h"
#include "DateTime.h"


RTC_SAMD51 rtc;
File myFile;

int READ = 1;
int WRITE = 0;

template <typename T>
void saveData(String sensorName, T *sensorData, int len){  //保存传感器数值到SD卡，参数：传感器名（同时也是文件名称），数据，数据长度
    String fileName = sensorName + ".csv";
    if(!SD.open(fileName, FILE_READ))             // header只写一次
    {
        myFile = SD.open(fileName, FILE_WRITE);    //追加写模式
        Serial.println("file does not found, creating...");
        myFile.print(sensorName + ",");
        myFile.print("timestamp,");
        for (int i = 0; i < len - 1; i++)
        {
            myFile.print("data[" + String(i+1) + "]");
            myFile.print(",");
        }
        myFile.println("data[" + String(len) + "]");
        myFile.close();
    }
    myFile = SD.open(fileName, FILE_APPEND);    //追加写模式
    myFile.print(sensorName);
    myFile.print(",");

    DateTime now = rtc.now();                   //读写当前时间
    myFile.print(String(now.year()) + "-" + String(now.month()) + "-" + String(now.day()) + " " + String(now.hour()) + ":" + String(now.minute()) + ":" + String(now.second()) + ",");
    
    for (int i = 0; i < len-1; i++)             //写入传感器值
    {
        myFile.print(sensorData[i]);
        Serial.println(sensorData[i]);
        myFile.print(",");
    }
    myFile.println(sensorData[len-1]);          //最后一个传感器数值加换行
    Serial.println(sensorData[len-1]);

    myFile.close();
}


void Readdata(String sensorName)
{
    CSV_Parser cp(/*format*/ "ssffff", /*has_header*/ true, /*delimiter*/ ',');

    String fileName = sensorName + ".csv";

    char arr[sensorName.length() + 1]; 
    strcpy(arr, sensorName.c_str());
    if (cp.readSDfile2(fileName)) {                         //读模式打开文件，读header
            char **column_1 = (char**)cp[arr];
            char **column_2 = (char**)cp["timestamp"];
            float *column_3 = (float*)cp["data1"];
            float *column_4 = (float*)cp["data2"];
            float *column_5 = (float*)cp["data3"];
            float *column_6 = (float*)cp["data4"];
            // float *column_7 = (float*)cp["data5"];

        for (int i = 0; i < cp.getRowsCount(); i++) {           //解析csv文件
            Serial.print(column_1[i]);             Serial.print(" - ");
            Serial.print(column_2[i]);             Serial.print(" - ");
            Serial.print(column_3[i], DEC);        Serial.print(" - ");
            Serial.print(column_4[i], DEC);        Serial.print(" - ");
            Serial.print(column_5[i], DEC);        Serial.print(" - ");
            Serial.print(column_6[i], DEC);        Serial.println();
            // Serial.print(column_6[i], DEC);        Serial.print(" - ");
            // Serial.print(column_7[i], DEC);        Serial.println();
        }
        
        // output parsed values (allows to check that the file was parsed correctly)
        cp.print(); // assumes that "Serial.begin()" was called before (otherwise it won't work)
    }
    else {
        Serial.println("ERROR: File does not exist...");
    }
}


void setup() {
    Serial.begin(115200);
    while(!Serial);

    rtc.begin();            //rtc初始化
    DateTime now = DateTime(F(__DATE__), F(__TIME__));
    rtc.adjust(now);
    now = rtc.now();

    if (!SD.begin(SDCARD_SS_PIN, SDCARD_SPI, 4000000UL)) {  //SD卡
        Serial.println("initialization failed!");
    }

    if(READ==1)Readdata("solil");   //读文件
}

void loop() {
    if(WRITE==1){
        int _size = 4;
        float data[_size] = {5, 6, 7, 10};

        saveData("solil", data, _size);  //写文件
        delay(5000);
    }
}