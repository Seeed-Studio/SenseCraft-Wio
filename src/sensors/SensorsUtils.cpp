
#include "Arduino.h"
#include "Wire.h"
#include "SoftwareI2C.h"
#include "sensor.h"

template <typename T> bool I2CScanner(uint8_t address, T wire) {
    uint8_t error = 0;
    wire.begin();
    wire.beginTransmission(address & 0x7F);

    error = wire.endTransmission();

    // features: for samd21 & samd51
    if (error == 0) {
        wire.beginTransmission(0x1);
        error = wire.endTransmission();
        if (error == 0) {
            wire.beginTransmission(0x7F);
            error = wire.endTransmission();
            if (error == 0)
                return false;
        }
    } else {
        return false;
    }

    return true;
}
template bool I2CScanner(uint8_t address, TwoWire wire);

template <typename T> bool I2CScanner(T wire) {
    for (unsigned char i = 1; i <= 127; i++) {
        if (wire.beginTransmission(i)) {
            // LOGSS.printf("Found I2C device at address: ");
            // LOGSS.println( i);
            return true;
        }
        wire.endTransmission();
    }
    // LOGSS.println(" not Found I2C device  ");
    return false;
}
template bool I2CScanner(SoftwareI2C wire);
