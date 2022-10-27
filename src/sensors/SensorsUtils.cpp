
#include "Arduino.h"
#include "Wire.h"
#include "SoftwareI2C.h"
#include "sensor.h"
#include "SensorsUtils.h"

SoftwareI2C softwarei2c;

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

/*************************************************************************************************
    Function Name: Grove_I2C_Check
    Description: check for i2c sensor connect
    Return: true: i2c sensor connected
*************************************************************************************************/
bool Grove_I2C_Check(SoftwareI2C& wire){
    wire.begin(SOFT_I2C_SDA, SOFT_I2C_SCL);
    for(int i=0; i<SENSOR_I2C_MAX; i++){
        if (I2C_Detect(grove_i2c_addr[i], wire)){
            // LOGSS.printf("detected sensor 0x");
            // LOGSS.println(grove_i2c_addr[i], HEX);
            return true;
        }
    }
    return false;
}