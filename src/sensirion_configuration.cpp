/*
    Copyright (c) 2017, Sensirion AG
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:

 * * Redistributions of source code must retain the above copyright notice, this
     list of conditions and the following disclaimer.

 * * Redistributions in binary form must reproduce the above copyright notice,
     this list of conditions and the following disclaimer in the documentation
     and/or other materials provided with the distribution.

 * * Neither the name of Sensirion AG nor the names of its
     contributors may be used to endorse or promote products derived from
     this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
    AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
    IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
    FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
    DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
    SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
    CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
    OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
    OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "i2c.h"
// needed for delay() routine
#include <Arduino.h>
#include <Wire.h>

/*
    INSTRUCTIONS
    ============

    Use the "arduino_example.ino" and copy
    sht.h, shtxx.c, sht_common.h, sht_common.c, i2c.h,
    hw_i2c/configuration.h, hw_i2c/configuration_Arduino.cpp
    in the .ino folder.
    Follow the function specification in the comments.
*/

#ifdef __cplusplus
extern "C" {
#endif

/**
    Initialize all hard- and software components that are needed for the I2C
    communication. After this function has been called, the functions
    i2c_read() and i2c_write() must succeed.
*/
void sensirion_i2c_init() {
    Wire.begin();
}

int8_t sensirion_i2c_read(uint8_t address, uint8_t* data, uint16_t count) {
    uint8_t readData[count];
    uint8_t rxByteCount = 0;

    // 2 bytes RH, 1 CRC, 2 bytes T, 1 CRC
    Wire.requestFrom(address, count);

    while (Wire.available()) { // wait till all arrive
        readData[rxByteCount++] = Wire.read();
        if (rxByteCount >= count) {
            break;
        }
    }

    memcpy(data, readData, count);

    return 0;
}

int8_t sensirion_i2c_write(uint8_t address, const uint8_t* data, uint16_t count) {
    Wire.beginTransmission(address);
    Wire.write(data, count);
    Wire.endTransmission();

    return 0;
}

/**
    Sleep for a given number of microseconds. The function should delay the
    execution for at least the given time, but may also sleep longer.

    @param useconds the sleep time in microseconds
*/
void sensirion_sleep_usec(uint32_t useconds) {
    delay((useconds / 1000) + 1);
}

#ifdef __cplusplus
} // extern "C"
#endif
