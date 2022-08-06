#ifndef __SENSECAP_H__
#define __SENSECAP_H__

#include <stdio.h>

//debug serial print
#define SENSECAP_DEBUG                          1

//pin


//sensecap
#define MEASURE_FIX                             0x00
#define MEASURE_POWER_TYPE                      0x07             
#define FULL_POWER                              0x6400
#define UPLOAD_INTERVAL                         0x0500
#define POWER_CRCL                              0x61
#define POWER_CRCH                              0x89

#define MEASURE_VERSION_TYPE                    0x00
#define HARDWARE_VER                            0x0101
#define SOFTWARE_VER                            0x0202
#define VER_CRCL                                0xC5
#define VER_CRCH                                0x56

#define MEASURE_BUITIN_DATA_TYPE                0x40
#define MEASURE_BUITIN_DATAOUT_TYPE             0x41
#define MEASURE_OTHER_DATA_TYPE                 0x42
#define MEASURE_VISIONAI_DATA1_TYPE             0x43
#define MEASURE_VISIONAI_DATAOUT_TYPE           0x44
#define MEASURE_VISIONAI_DATA2_TYPE             0x45

#define NO_DATA                                 0xFF
#define FREQUENCY                               DSKLORAE5_ZONE_EU868

#pragma pack(1)

/*
+-------------+------+-------------+---------------+-----------------+------------------+-----------------+
|     fix     | type |     fix     | battery level | upload interval |      CRC_L       |      CRC_L      |
+-------------+------+-------------+---------------+-----------------+------------------+-----------------+
|      8      |   8  |      8      |       16      |       16        |        8         |        8        |
+-------------+------+-------------+---------------+-----------------+------------------+-----------------+
 fix : 固定为0x00
 type : 0x07
 fix : 固定为0x00
 battery level : 电池电量0x64 0x00
 upload interval : 周期上报间隔0x05 0x00
 CRC: 校验码0x61 0x89
 */
typedef struct power_data
{
    uint8_t fix_1;
    uint16_t type;
    uint8_t fix_2;
    uint16_t battery_level;
    uint16_t upload_interval;
    uint8_t CRC_L;
    uint8_t CRC_H;
} power_data_t;


/*
+-------+-------------+------------------+-------------------+---------------+---------------+
| type  |     fix     | hardware version | software version  |     CRC_L     |     CRC_H     |
+-------+-------------+------------------+-------------------+---------------+---------------+
|  16   |      8      |        16        |       16          |       8       |       8       |
+-------+-------------+------------------+-------------------+---------------+---------------+
 type : 0x00 0x00
 fix : 固定为0x00
 hardware version : 硬件版本号0x01 0x01
 software version : 软件版本号0x02 0x02
 CRC: 校验码0xC5 0x56
 */
typedef struct version_data
{
    uint16_t type;
    uint8_t fix;
    uint16_t hardware_version;
    uint16_t software_version;
    uint8_t CRC_L;
    uint8_t CRC_H;
} version_data_t;

/*
+------+-------------+-------------+-------------+-------------+-------------+-------------+-------------+-------------+-------------+-------------+
| type | Light Data1 | Light Data2 | Sound Data1 | Sound Data2 | IMU_x Data1 | IMU_x Data2 | IMU_y Data1 | IMU_y Data2 | IMU_z Data1 | IMU_z Data2 |
+------+-------------+-------------+-------------+-------------+-------------+-------------+-------------+-------------+-------------+-------------+
|   8  |      8      |      8      |      8      |      8      |      8      |      8      |      8      |      8      |      8      |      8      |
+------+-------------+-------------+-------------+-------------+-------------+-------------+-------------+-------------+-------------+-------------+
 type : 0x40-数据上报首帧（内置传感器）, 0x41-输出帧
 Data : 0xFF表示无数据，传感器依次为：光线、响度、IMU_x、IMU_y、IMU_z
 */
typedef struct builtin_sensor_data
{
    uint8_t type;
    uint8_t sensor_data[10];
} builtin_sensor_data_t;

/*
+------+-------------+-------------+-------------+-------------+-------------+-------------+-------------+-------------+-------------+-------------+
| type | Temp Data1  | Temp Data2  | Humi Data1  | Humi Data2  | tVOC Data1  | tVOC Data2  | CO2eq Data1 | CO2eq Data2 | soil Data1  | soil Data2  |
+------+-------------+-------------+-------------+-------------+-------------+-------------+-------------+-------------+-------------+-------------+
|   8  |      8      |      8      |      8      |      8      |      8      |      8      |      8      |      8      |      8      |      8      |
+------+-------------+-------------+-------------+-------------+-------------+-------------+-------------+-------------+-------------+-------------+
 type : 0x41-输出帧（套件内其他传感器数据采集）
 Data : 0xFF表示无数据，传感器依次为SHT40、SGP30、Soil Moisture
 */
typedef struct other_sensor_data
{
    uint8_t type;
    uint8_t sensor_data[10];
} other_sensor_data_t;

/*
+------+-------------+-------------+-------------+-------------+-------------+-------------+-------------+-------------+-------------+-------------+
| type | Peole Data1 | Peole Data2 | Peole Data1 | Peole Data2 | Peole Data1 | Peole Data2 | Peole Data1 | Peole Data2 | Peole Data1 | Peole Data2 |
+------+-------------+-------------+-------------+-------------+-------------+-------------+-------------+-------------+-------------+-------------+
|   8  |      8      |      8      |      8      |      8      |      8      |      8      |      8      |      8      |      8      |      8      |
+------+-------------+-------------+-------------+-------------+-------------+-------------+-------------+-------------+-------------+-------------+
 type : 0x42-第一组识别5个人的置信度上报，0x43-输出帧
 Data : 0xFF表示无数据
 */
typedef struct visionai_sensor_data1
{
    uint8_t type;
    uint8_t sensor_data[10];
} visionai_sensor_data1_t;

/*
+------+-------------+-------------+-------------+-------------+-------------+-------------+-------------+-------------+-------------+-------------+
| type | Peole Data1 | Peole Data2 | Peole Data1 | Peole Data2 | Peole Data1 | Peole Data2 | Peole Data1 | Peole Data2 | Peole Data1 | Peole Data2 |
+------+-------------+-------------+-------------+-------------+-------------+-------------+-------------+-------------+-------------+-------------+
|   8  |      8      |      8      |      8      |      8      |      8      |      8      |      8      |      8      |      8      |      8      |
+------+-------------+-------------+-------------+-------------+-------------+-------------+-------------+-------------+-------------+-------------+
 type : 0x43-输出帧（识别的第6~10个人置信度上报）
 Data : 0xFF表示无数据
 */
typedef struct visionai_sensor_data2
{
    uint8_t type;
    uint8_t sensor_data[10];
} visionai_sensor_data2_t;


typedef struct dtu_data_list
{
    uint8_t len;
    union 
    {
        builtin_sensor_data_t builtin_sensor;
        other_sensor_data_t other_sensor;
    };
} dtu_data_list_t;

// void data_decord(int vals, uint8_t *data, int signal);

// class SenseCAP{
//     private:
//         bool lora_statue_begin = false;
//         bool lora_statue_setup = false;
//     public:
//         void Read_builtin();
//         void Lora_Init();
//         void Lora_SendData(uint8_t channel, uint8_t *data);
//         void IMU_init();
// };

#endif