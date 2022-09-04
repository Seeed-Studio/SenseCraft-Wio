#ifndef __UTILS_H__
#define __UTILS_H__
#include <stdint.h>

#define LOGSS Serial1
enum button_state {
    NETWORK_PRESSED,
    PROCESS_PRESSED,
    SENSE_PRESSED,
    UP_PRESSED,
    DOWN_PRESSED,
    LEFT_PRESSED,
    RIGHT_PRESSED,
    SELECT_PRESSED,
};

enum lora_status_type {
    LORA_INIT_START,
    LORA_INIT_FAILED,
    LORA_INIT_SUCCESS,
    LORA_JOIN_FAILED,
    LORA_JOIN_SUCCESS,
    LORA_SEND_FAILED,
    LORA_SEND_SUCCESS,
};

enum sensor_data_type {
    SENSOR_DATA_TYPE_INT32 = 0,
    SENSOR_DATA_TYPE_FLOAT,
};

enum sensor_ui_type {
    SENSOR_UI_TYPE_NORMAL = 0,
    SENSOR_UI_TYPE_AVERAGE,
};

struct sensor_data {
    const char   *name;
    const char   *data_unit;
    const void   *data;
    unsigned char size;
    uint8_t       id;
    bool          status;        // 0: normal, 1: error
    uint8_t       data_type = 0; // 0: int32_t, 1: float(*100)
    uint8_t       ui_type   = 0; // 0: normal, 1: average value
};

struct log_data {
    char     data[64];
    uint16_t time;
};

enum sensor_type {
    // 1-3, buildin sensor
    BUILDIN_LIGHT = 1,
    BUILDIN_MIC,
    LIS3DHTRSENSOR,
    GROVE_VISIONAI,
    GROVE_SHT4X,
    GROVE_SGP30,
    GROVE_SOIL,
};

enum lora_freq {
    UNDEFINED,
    EU868,
    US915,
    AS923_1,
    AS923_2,
    AS923_3,
    AS923_4,
    KR920,
    IN865,
    AU915,
};

#endif // __UTILS_H__
