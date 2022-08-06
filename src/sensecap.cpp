#include "sensecap.h"
#include "disk91_LoRaE5.h"
#include "LIS3DHTR.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Wire.h>

// char deveui[] = "1CF7F1C043200033";
// char appeui[] = "8000000000000009";
// char appkey[] = "92C03BCBA4197FBD2BC86E27B6113AFC";

// LIS3DHTR<TwoWire> lis;
// Disk91_LoRaE5 lorae5(&Serial);

power_data_t power_val = {
    .fix_1 = MEASURE_FIX,
    .type = MEASURE_POWER_TYPE,
    .fix_2 = MEASURE_FIX,
    .battery_level = FULL_POWER,
    .upload_interval = UPLOAD_INTERVAL,
    .CRC_L = POWER_CRCL,
    .CRC_H = POWER_CRCH,
};

version_data_t version_val = {
    .type = MEASURE_VERSION_TYPE,
    .fix = MEASURE_FIX,
    .hardware_version = HARDWARE_VER,
    .software_version = SOFTWARE_VER,
    .CRC_L = VER_CRCL,
    .CRC_H = VER_CRCH,
};

builtin_sensor_data_t builtin_sensor = {
    .type = MEASURE_BUITIN_DATA_TYPE,
};

other_sensor_data_t other_sensor = {
    .type = MEASURE_OTHER_DATA_TYPE,
};

visionai_sensor_data1_t visionai_data1 = {
    .type = MEASURE_VISIONAI_DATA1_TYPE,
};

visionai_sensor_data2_t visionai_data2 = {
    .type = MEASURE_VISIONAI_DATA2_TYPE,
};


// //传感器数据值处理(大端模式)
// void data_decord(int vals, uint8_t *data, int signal)
// {
//     if (vals > 0 || vals == 0)
//     {
//         Serial.println(data[signal]);
//         data[signal] = vals >> 8 & 0xFF;
//         data[signal + 1] = vals & 0xFF;
//     }
//     else{
//         vals = abs(vals);
//         vals = ~vals+1;
//         data[signal] = vals >> 8 & 0xFF;
//         data[signal + 1] = vals & 0xFF;
//     }
// }

// //lora e5初始化
// void SenseCAP::Lora_Init()
// {
//     // init the library, search the LORAE5 over the different WIO port available
//     if ( lorae5.begin(DSKLORAE5_SWSERIAL_WIO_P2) ) {
//         lora_statue_begin = true;
//     }
//     #if SENSECAP_DEBUG   
//         if(SENSECAP_DEBUG)
//             // Setup the LoRaWan Credentials
//             if (!lorae5.setup(
//                     FREQUENCY,
//                     deveui,
//                     appeui,
//                     appkey))
//             { // Setup the LoRaWAN stack with the stored credentials
//                 lora_statue_setup = true;
//             }
//     #else
//         // Setup the LoRaWan Credentials
//         if ( ! lorae5.setup_sensecap(
//                 FREQUENCY,
//             ) ){
//             lora_statue_setup = true;
//         }
//     #endif
// }


// void SenseCAP::Lora_SendData(uint8_t channel, uint8_t *data)
// {
//     //uint8_t data1[] = { 0x01, 0x02, 0x03, 0x04 };
//     uint8_t rxBuff[16];
//     uint8_t rxSize = 16;
//     uint8_t rxPort;
//     if(lorae5.join_sync())
//     {
//         while ( !lorae5.sendReceive_sync(channel, data, sizeof(data), rxBuff, &rxSize, &rxPort, 7, 14, 0) ) 
//         {
//             delay(10000);
//         }
//         #if SENSECAP_DEBUG
//             Serial.println("Uplink done");
//         #endif
//             delay(10000);
//     }
// }

// void SenseCAP::IMU_init()
// {
//     lis.begin(Wire1);
//     lis.setOutputDataRate(LIS3DHTR_DATARATE_25HZ); //Data output rate
//     lis.setFullScaleRange(LIS3DHTR_RANGE_2G);
// }


// //初始化内置传感器，读内置传感器的值
// void SenseCAP::Read_builtin()
// {
//     builtin_sensor_data *builtin;
//     Serial.println("light");
//     int light = analogRead(WIO_LIGHT);
//     Serial.println(light);
//     data_decord(light, builtin->sensor_data, 1);
//     Serial.println("mic");
//     int mic_val = analogRead(WIO_MIC);
//     data_decord(mic_val, builtin->sensor_data, 3);
//     Serial.println("IMU");
//     float x_values, y_value, z_val;
//     lis.getAcceleration(&x_values, &y_value, &z_val);
//     int x = x_values*100;
//     int y = y_value*100;
//     int z = z_val*100;
    
//     data_decord(x, builtin->sensor_data, 5);
//     data_decord(y, builtin->sensor_data, 7);
//     data_decord(z, builtin->sensor_data, 9);

//     Lora_SendData(2, builtin->sensor_data);

//     #if SENSECAP_DEBUG
//         Serial.println(light);
//         Serial.println(mic_val);
//         Serial.println(x);
//         Serial.println(y);
//         Serial.println(z);
//     #endif
// }


