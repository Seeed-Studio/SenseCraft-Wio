#ifndef SEEED_SDCARD_HAL_H
#define SEEED_SDCARD_HAL_H


#include "SPI.h"
#include <Seeed_FS.h>
#include "Arduino.h"

typedef enum {
    CARD_NONE,
    CARD_MMC,
    CARD_SD,
    CARD_SDHC,
    CARD_UNKNOWN
} sdcard_type_t;

typedef struct {
    uint8_t ssPin;
    SPIClass* spi;
    int frequency;
    sdcard_type_t type;
    unsigned long sectors;
    bool supports_crc;
    int status;
} ardu_sdcard_t;

uint8_t sdcard_init(uint8_t cs, SPIClass* spi, int hz);
uint8_t sdcard_uninit(uint8_t pdrv);

bool sdcard_mount(uint8_t pdrv);
uint8_t sdcard_unmount(uint8_t pdrv);

sdcard_type_t sdcard_type(uint8_t pdrv);
uint32_t sdcard_num_sectors(uint8_t pdrv);
uint32_t sdcard_sector_size(uint8_t pdrv);

#endif