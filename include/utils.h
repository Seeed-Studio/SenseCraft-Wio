#ifndef __UTILS_H__
#define __UTILS_H__
#include <stdint.h>
enum button_state {
    SENSE_PRESSED = 1,
    PROCESS_PRESSED,
    NETWORK_PRESSED,
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
    BUILDIN_LIGHT = 1,
    BUILDIN_MIC,
    LIS3DHTRSENSOR,
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
