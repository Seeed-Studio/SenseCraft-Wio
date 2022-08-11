/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2016        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/
#ifndef ARDUINO_ARCH_NRF52840
#include "ff.h"      /* Obtains integer types */
#include "diskio.h"   /* Declarations of disk functions */
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "integer.h"

static ff_diskio_impl_t * s_impls[_VOLUMES] = { NULL };

#if _MULTI_PARTITION    /* Multiple partition configuration */
PARTITION VolToPart[] = {
    {0, 0},    /* Logical drive 0 ==> Physical drive 0, auto detection */
    {1, 0}     /* Logical drive 1 ==> Physical drive 1, auto detection */
};
#endif


uint8_t ff_diskio_get_drive(BYTE* out_pdrv)
{
    BYTE i;
    for(i=0; i<_VOLUMES; i++) {
        if (!s_impls[i]) {
            *out_pdrv = i;
            return 0;
        }
    }
    return 1;
}

void ff_diskio_register(BYTE pdrv, const ff_diskio_impl_t* discio_impl)
{
    assert(pdrv < _VOLUMES);

    if (s_impls[pdrv]) {
        ff_diskio_impl_t* im = s_impls[pdrv];
        s_impls[pdrv] = NULL;
        free(im);
    }

    if (!discio_impl) {
        return;
    }

    ff_diskio_impl_t * impl = (ff_diskio_impl_t *)malloc(sizeof(ff_diskio_impl_t));
    assert(impl != NULL);
    memcpy(impl, discio_impl, sizeof(ff_diskio_impl_t));
    s_impls[pdrv] = impl;
}

DSTATUS disk_initialize (BYTE pdrv)
{
    return s_impls[pdrv]->init(pdrv);
}
DSTATUS disk_status (BYTE pdrv)
{
    return s_impls[pdrv]->status(pdrv);
}
DRESULT disk_read (BYTE pdrv, BYTE* buff, DWORD sector, UINT count)
{
    return s_impls[pdrv]->read(pdrv, buff, sector, count);
}
DRESULT disk_write (BYTE pdrv, const BYTE* buff, DWORD sector, UINT count)
{
    return s_impls[pdrv]->write(pdrv, buff, sector, count);
}
DRESULT disk_ioctl (BYTE pdrv, BYTE cmd, void* buff)
{
    return s_impls[pdrv]->ioctl(pdrv, cmd, buff);
}
#endif
