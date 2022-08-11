/*
    a wrapper for SDCard.

    Created by Hongtai.liu 13 July 2019

*/

#include "Seeed_SD.h"
#include "Seeed_sdcard_hal.h"
#include <Seeed_FS.h>

namespace fs
{

    boolean SDFS::begin(uint8_t ssPin, SPIClass &spi, int hz)
    {
        _pdrv = sdcard_init(ssPin, &spi, hz);
        spi.begin();
        FRESULT status;
        _drv[0] = _T('0' + _pdrv);
        status = f_mount(&root, _drv, 1);
        if (status != FR_OK)
        {
            return false;
        }
        else
        {
            return true;
        }
    }

    void SDFS::end()
    {
        if (_pdrv != 0xFF)
        {
            f_mount(NULL, _drv, 1);
            sdcard_uninit(_pdrv);
            _pdrv = 0xFF;
        }
    }

    sdcard_type_t SDFS::cardType()
    {
        if (_pdrv == 0xFF)
        {
            return CARD_NONE;
        }
        return sdcard_type(_pdrv);
    }

    uint64_t SDFS::cardSize()
    {
        if (_pdrv == 0xFF)
        {
            return 0;
        }
        size_t sectors = sdcard_num_sectors(_pdrv);
        size_t sectorSize = sdcard_sector_size(_pdrv);
        return (uint64_t)sectors * sectorSize;
    }

    uint64_t SDFS::totalBytes()
    {
        FATFS *fsinfo;
        DWORD fre_clust;
        if (f_getfree(_drv, &fre_clust, &fsinfo) != 0)
        {
            return 0;
        }
        uint64_t size = ((uint64_t)(fsinfo->csize)) * (fsinfo->n_fatent - 2)
#if _MAX_SS != 512
                        * (fsinfo->ssize);
#else
                        * 512;
#endif
        return size;
    }

    uint64_t SDFS::usedBytes()
    {
        FATFS *fsinfo;
        DWORD fre_clust;
        if (f_getfree(_drv, &fre_clust, &fsinfo) != 0)
        {
            return 0;
        }
        uint64_t size = ((uint64_t)(fsinfo->csize)) * ((fsinfo->n_fatent - 2) - (fsinfo->free_clst))
#if _MAX_SS != 512
                        * (fsinfo->ssize);
#else
                        * 512;
#endif
        return size;
    }

    SDFS SD;
};
