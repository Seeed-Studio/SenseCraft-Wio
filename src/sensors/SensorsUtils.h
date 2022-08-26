
#ifndef __SENSORSUTILS_H__
#define __SENSORSUTILS_H__
#include "Arduino.h"
#include "Wire.h"

#define SENSOR_READ_DELAY 200
//Scan the specified address I2C device to determine whether it is connected.
bool I2CScanner(uint8_t address, TwoWire &wire = Wire);

#endif // __SENSORSUTILS_H__