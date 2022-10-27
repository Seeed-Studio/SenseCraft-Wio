
#ifndef __SENSORSUTILS_H__
#define __SENSORSUTILS_H__
#include "Arduino.h"
#include "Wire.h"
#include "SoftwareI2C.h"

#define SENSOR_READ_DELAY 1000

#define SOFT_I2C_SDA D1
#define SOFT_I2C_SCL D0
// #define I2C_ADD_AIVISION (0x62)
// #define I2C_ADDR_SGP30   (0x58)
// #define I2C_ADDR_SHT4X   (0x44) // Type B = 0x45
// #define IIC_ADDR_VL53LX0 (0x29)
#define SENSOR_I2C_MAX 4
enum grove_i2c_index{S_VISION, S_SGP30, S_SHT4X, S_VL53LX0};
const uint8_t grove_i2c_addr[4] = { 0x62, 0x58, 0x44, 0x29};
extern SoftwareI2C softwarei2c;

inline bool I2C_Detect(uint8_t address, SoftwareI2C wire) {
    bool ret = wire.beginTransmission(address);
    wire.endTransmission();
    return ret;
}
bool Grove_I2C_Check(SoftwareI2C& wire);
//Scan the specified address I2C device to determine whether it is connected.
template <typename T> bool I2CScanner(uint8_t address, T wire);
template <typename T> bool I2CScanner(T wire);
#endif // __SENSORSUTILS_H__