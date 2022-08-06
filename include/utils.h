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

enum sensor_type {
    BUILDIN_LIGHT = 1,
    BUILDIN_MIC,
};

#endif // __UTILS_H__
