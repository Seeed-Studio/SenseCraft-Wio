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

struct sensor_data
{
	const char *name;
	const void *data;
	unsigned char size;
    uint8_t id;
    bool status; // 0: normal, 1: error
};

enum sensor_type {
    BUILDIN_LIGHT = 1,
    BUILDIN_MIC,
    LIS3DHTRSENSOR,
};

#endif // __UTILS_H__
