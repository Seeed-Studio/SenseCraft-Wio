#ifdef ARDUINO_ARCH_NRF52840
/* use for NRF52840(eg. XIAO BLE) */
#include <SD.h>

#else

#include "Seeed_FS.h"
#include "SD/Seeed_SD.h"

#ifdef WIO_LITE_AI
#include "SDMMC/Seeed_SDMMC.h"
#endif

#endif