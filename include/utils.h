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

struct sensor_data {
    const char   *name;
    const void   *data;
    unsigned char size;
    uint8_t       id;
    bool          status; // 0: normal, 1: error
};

enum sensor_type {
    //1-3, buildin sensor
    BUILDIN_LIGHT = 1,
    BUILDIN_MIC,
    LIS3DHTRSENSOR,
    BUILDEX_VISIONAI,
    BUILDEX_SHT4X,
    BUILDEX_SGP30,
    BUILDEX_SOIL,
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
