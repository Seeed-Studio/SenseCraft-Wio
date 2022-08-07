
#include "Arduino.h"
#include "Wire.h"

bool I2CScanner(uint8_t address, TwoWire &wire)
{
    uint8_t error = 0;
    wire.begin();
    wire.beginTransmission(address&0x7F);

    error = wire.endTransmission();

    //features: for samd21 & samd51
    if (error == 0)
    {
        wire.beginTransmission(0x1);
        error = wire.endTransmission();
        if (error == 0)
        {
            wire.beginTransmission(0x7F);
            error = wire.endTransmission();
            if(error == 0)
                return false;
        }
    }else{
        return false;
    }

   return true;
}
