/*

    SD - a slightly more friendly wrapper for sdfatlib

    This library aims to expose a subset of SD card functionality
    in the form of a higher level "wrapper" object.

    License: GNU General Public License V3
          (Because sdfatlib is licensed with this.)

    (C) Copyright 2010 SparkFun Electronics

*/

#ifndef __SD_H__
#define __SD_H__

#include <Arduino.h>
#include "Seeed_sdcard_hal.h"
#include <Seeed_FS.h>
#include <SPI.h>



namespace fs {

    class SDFS : public FS {
      public:
        SDFS() {}
        ~SDFS() {}
        // This needs to be called to set up the connection to the SD card
        // before other methods are used.
        boolean begin(uint8_t ssPin = 11, SPIClass& spi = SPI, int hz = 4000000);

        //call this when a card is removed. It will allow you to insert and initialise a new card.
        void end();

        sdcard_type_t cardType();
        uint64_t cardSize();
        uint64_t totalBytes();
        uint64_t usedBytes();
    };

    extern SDFS SD;
};

using namespace fs;
typedef fs::File        SDFile;
typedef fs::SDFS        SDFileSystemClass;
#define SDFileSystem    SD


#endif
