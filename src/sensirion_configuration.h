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

#ifndef SENSIRION_CONFIGURATION_H
#define SENSIRION_CONFIGURATION_H

#ifdef __cplusplus
extern "C" {
#endif

/**
    The I2C address of the SGP sensor. Only uncomment if you have a special
    version of a sensor.
*/
/* #define SGP_ADDRESS 0x58 */

/**
    The clock period of the i2c bus in microseconds. Increase this, if your GPIO
    ports cannot support a 200 kHz output rate.
*/
#define I2C_CLOCK_PERIOD_USEC 10

/**
    Typedef section for types commonly defined in <stdint.h>
    You are welcome to use #include <stdint.h> instead and discard
    the list by uncommenting the following #define. In this case, please make
    sure that int64_t and uint64_t are also defined!
*/
#include <stdint.h>
#define USE_SENSIRION_STDINT_TYPES 0

/**
    Uncomment the following line to enable clock stretching.
    Note that clock stretching will keep the bus busy until the sensor releases
    it and will impact the communication with other i2c devices on the same bus.
*/
/* #define USE_SENSIRION_CLOCK_STRETCHING 1 */

/**
    Set USE_SENSIRION_STDINT_TYPES to 0 if your platform already has a stdint
    implementation.
*/
#ifndef USE_SENSIRION_STDINT_TYPES
#define USE_SENSIRION_STDINT_TYPES 1
#endif /* USE_SENSIRION_STDINT_TYPES */

/**
    Set USE_SENSIRION_SHORT_TYPES to 0 if your platform already provides the
    type definitions u8, u16, u32, u64, s8, s16, s32, s64
*/
#ifndef USE_SENSIRION_SHORT_TYPES
#define USE_SENSIRION_SHORT_TYPES 1
#endif /* USE_SENSIRION_SHORT_TYPES */

#if USE_SENSIRION_STDINT_TYPES
typedef unsigned long long int uint64_t;
typedef long long int int64_t;
typedef long int32_t;
typedef unsigned long uint32_t;
typedef short int16_t;
typedef unsigned short uint16_t;
typedef char int8_t;
typedef unsigned char uint8_t;
#endif /* USE_SENSIRION_STDINT_TYPES */

#if USE_SENSIRION_SHORT_TYPES
typedef int64_t s64;
typedef uint64_t u64;
typedef int32_t s32;
typedef uint32_t u32;
typedef int16_t s16;
typedef unsigned short u16;
typedef int8_t s8;
typedef uint8_t u8;
#endif

#ifndef USE_SENSIRION_CLOCK_STRETCHING
#define USE_SENSIRION_CLOCK_STRETCHING 0
#endif /* USE_SENSIRION_CLOCK_STRETCHING */

#ifdef __cplusplus
}
#endif

#endif /* SENSIRION_CONFIGURATION_H */
