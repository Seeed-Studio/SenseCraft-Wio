
#ifndef __SENSORSUTILS_H__
#define __SENSORSUTILS_H__
#include "Arduino.h"
#include "Wire.h"
#include "SoftwareI2C.h"

#define SENSOR_READ_DELAY 200
//Scan the specified address I2C device to determine whether it is connected.
template <typename T> bool I2CScanner(uint8_t address, T wire);
template <typename T> bool I2CScanner(T wire);
#endif // __SENSORSUTILS_H__