#include <Arduino.h>
#include <TFT_eSPI.h>
#include <Wire.h>
#include "disk91_LoRaE5.h"
#include "LIS3DHTR.h"
#include "sgp30.h"
#include "sensirion_common.h"
#include <SensirionI2CSht4x.h>
#include "Seeed_Arduino_GroveAI.h"

#define Frequency DSKLORAE5_ZONE_EU868

LIS3DHTR<TwoWire> lis;
SensirionI2CSht4x sht4x;
GroveAI ai(Wire);

//Grove LoRa E5
Disk91_LoRaE5 lorae5(&Serial); // Where the AT command and debut traces are printed

char deveui[] = "1CF7F1C043200033";
char appeui[] = "8000000000000009";
char appkey[] = "92C03BCBA4197FBD2BC86E27B6113AFC";

//全局变量
int light=0, mic_val=0;  //光线值、麦克风响度
float x_values=0.0, y_value=0.0, z_val=0.0; //IMU数值
u16 tvoc_ppb, co2_eq_ppm; //sgp30
int humi, temp;  //sht40
int soilValue = 0;  //soil
int soilPin = A0;

//传感器状态
bool sgp30_statue = false;
bool sht40_statue = false;
bool soil_statue = false;
bool visionai_statue = false;
bool lora_first_data = false;

//SenseCAP
unsigned char Built_in[11] = {0x40, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
unsigned char Other_sensor[11] = {0x42, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
unsigned char VisionAI_data1[11] = {0x43, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
unsigned char VisionAI_data2[11] = {0x45, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

//传感器数据值处理
void data_decord(int vals[5], unsigned char data[11])
{
  for(int i = 0, j = 1; i<5; i++,j+=2)
  {
    if(vals[i]>0 || vals[i]==0){
      data[j] = vals[i] >> 8 & 0xFF;
      data[j+1] = vals[i] & 0xFF;
    }
    else{
      vals[i] = abs(vals[i]);
      vals[i] = ~vals[i]+1;
      data[j] = vals[i] >> 8 & 0xFF;
      data[j+1] = vals[i] & 0xFF;
    }
  }
}

//内置传感器读值任务
void read_sensor()
{
  //built-in
  light = analogRead(WIO_LIGHT);
  // Serial.println(light);
  mic_val = analogRead(WIO_MIC);
  // Serial.println(mic_val);
  lis.getAcceleration(&x_values, &y_value, &z_val);
  int x = x_values*100;
  int y = y_value*100;
  int z = z_val*100;
  // Serial.println(x);
  // Serial.println(y);
  // Serial.println(z);

  //sgp30
  if(sgp30_statue)
  {
    Serial.println("sgp30 on");
    s16 err = 0;
    u16 scaled_ethanol_signal, scaled_h2_signal;
    // /*Read H2 and Ethanol signal in the way of blocking*/
    err = sgp_measure_signals_blocking_read(&scaled_ethanol_signal, &scaled_h2_signal);
    // Set absolute humidity to 13.000 g/m^3
    // It's just a test value
    sgp_set_absolute_humidity(13000);
    err = sgp_iaq_init();
    err = sgp_measure_iaq_blocking_read(&tvoc_ppb, &co2_eq_ppm);
  }
  else
  {
    Serial.println("sgp30 off");
    tvoc_ppb = 0xFFFF;
    co2_eq_ppm = 0xFFFF;
  }
  
  //sht40
  if(sht40_statue)
  {
    Serial.println("sht40 on");
    float temperature, humidity;
    uint16_t error;
    error = sht4x.measureHighPrecision(temperature, humidity);
    Serial.print("temperature  ");
    Serial.println(temperature);
    temp = temperature * 100;
    humi = humidity*100;
  }
  else
  {
    Serial.println("sht40 off");
    temp = 0xFFFF;
    humi = 0xFFFF;
  }

  //soil moisture
  if(soil_statue)
  {
    Serial.println("soil sensor on");
    soilValue = analogRead(soilPin);
  }
  else
  {
    Serial.println("soil sensor off");
    soilValue = 0xFFFF;
  }

  //visionai
  if(visionai_statue)
  {
    Serial.println("visionai on");
    if (ai.invoke()) // begin invoke
    {
      while (1) // wait for invoking finished
      {
        CMD_STATE_T ret = ai.state(); 
        if (ret == CMD_STATE_IDLE)
        {
          break;
        }
        delay(20);
      }
      uint8_t len = ai.get_result_len(); // receive how many people detect
      Serial.println(len);
      if(len)
      {
        object_detection_t data;       //get data

        for (int i = 0; i < len; i++)
        {
          ai.get_result(i, (uint8_t*)&data, sizeof(object_detection_t)); //get result
          int conf = data.confidence;
          Serial.println(conf);
          if(i<5)
          {
            VisionAI_data1[2*i+1] = 1;
            VisionAI_data1[2*i+2] = conf;
          }
          else if(i>4)
          {
            if(i>9)break;
            VisionAI_data2[2*i-9] = 1;
            VisionAI_data2[2*i-8] = conf;
          }
        }
      }
      else
      {
        Serial.println("no people");
        for (int i = 1; i < 11; i++)
        {
          VisionAI_data1[i] = 0xFF;
          VisionAI_data2[i] = 0xFF;
        }
      }
    }
    else
    {
      for (int i = 1; i < 11; i++)
      {
        VisionAI_data1[i] = 0xFF;
        VisionAI_data2[i] = 0xFF;
      }
    }
  }
  else
  {
    Serial.println("visionai off");
    for (int i = 1; i < 11; i++)
    {
      VisionAI_data1[i] = 0xFF;
      VisionAI_data2[i] = 0xFF;
    }
  }

  int data1[5] = {light, mic_val, x, y, z};
  int data2[5] = {temp, humi, tvoc_ppb, co2_eq_ppm, soilValue};

  data_decord(data1, Built_in);
  data_decord(data2, Other_sensor);

}

//lora e5初始化
void Lora_Init()
{
  
  // init the library, search the LORAE5 over the different WIO port available
  if ( ! lorae5.begin(DSKLORAE5_SWSERIAL_WIO_P2) ) {
    Serial.println("LoRa E5 Init Failed");
    while(1); 
  }
  
  // Setup the LoRaWan Credentials
  if ( ! lorae5.setup(
        Frequency,
        deveui,
        appeui,
        appkey
     ) ){   // Setup the LoRaWAN stack with the stored credentials
    Serial.println("LoRa E5 Setup Failed");
    while(1);
  }
}

//lora e5连接LoRa并且发送数据
void Join_sent_data()
{ 
    uint8_t rxBuff[16];
    uint8_t rxSize = 16;
    uint8_t rxPort;
    if ( lorae5.join_sync() )
    {
      Serial.println("Success in the LoRa");
      if( lora_first_data )
      {
        static uint8_t data_1[9] = { 0x00, 0x07, 0x00, 0x64, 0x00, 0x05, 0x00, 0x61, 0x89 };
        while ( !lorae5.sendReceive_sync( 2, data_1, sizeof(data_1),rxBuff,&rxSize,&rxPort,7,14,0) ) 
        {
          delay(10000);
        }
        Serial.println("Uplink Electricity information done");
        delay(10000);
        static uint8_t data_2[9] = { 0x00, 0x00, 0x00, 0x01, 0x01, 0x02, 0x02, 0xC5, 0x56 };
        while ( !lorae5.sendReceive_sync( 2, data_2, sizeof(data_2),rxBuff,&rxSize,&rxPort,7,14,0) ) 
        {
          delay(10000);
        }
        Serial.println("Uplink Version information done");
        lora_first_data = true;
        delay(10000);
      }
      
      while ( !lorae5.sendReceive_sync( 3, Built_in, sizeof(Built_in),rxBuff,&rxSize,&rxPort,7,14,0) ) 
      {
        delay(10000);
      }
      Serial.println("Uplink Built-in Sensor.");
      delay(10000);

      while ( !lorae5.sendReceive_sync( 3, Other_sensor, sizeof(Other_sensor),rxBuff,&rxSize,&rxPort,7,14,0) ) 
      {
        delay(10000);
      }
      Serial.println("Uplink Other Sensor.");
      delay(10000);

      while ( !lorae5.sendReceive_sync( 3, VisionAI_data1, sizeof(VisionAI_data1),rxBuff,&rxSize,&rxPort,7,14,0) ) 
      {
        delay(10000);
      }
      Serial.println("Uplink VisionAI data1.");
      delay(10000);

      while ( !lorae5.sendReceive_sync( 3, VisionAI_data2, sizeof(VisionAI_data2),rxBuff,&rxSize,&rxPort,7,14,0) ) 
      {
        delay(10000);
      }
      Serial.println("Uplink VisionAI data2.");
      delay(10000);
      
//      Serial.println("This transmission ends and the next round of data transmission begins in five minutes.");
//      delay(300000);
    }
}

void sensor_init()
{
  Wire.begin();

  //IMU
  lis.begin(Wire1);
  lis.setOutputDataRate(LIS3DHTR_DATARATE_25HZ); //Data output rate
  lis.setFullScaleRange(LIS3DHTR_RANGE_2G);

  //SGP30
  if (sgp_probe() != STATUS_OK)
    sgp30_statue = true;
  else
    sgp30_statue = false;

  //sht40
  sht4x.begin(Wire);
  uint32_t serialNumber;
  uint16_t error = sht4x.serialNumber(serialNumber);
  if (!error)
    sht40_statue = true;
  else
    sht40_statue = false;

  //VisionAI
  if (ai.begin(ALGO_OBJECT_DETECTION, MODEL_EXT_INDEX_1))
    visionai_statue = true;
  else
    visionai_statue = false;

  //soil
  // if( analogRead(soilPin) )
  //   soil_statue = true;
  // else
  //   soil_statue = false;
}

void setup() 
{
  // put your setup code here, to run once:
  Serial.begin(9600);
  while (!Serial) ;
  Lora_Init();
}

void loop() 
{
  // put your main code here, to run repeatedly:
  sensor_init();
  read_sensor();
  Join_sent_data();
}
