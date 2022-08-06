#include "system.h"

static int32_t msc_read_cb(uint32_t lba, void *buffer, uint32_t bufsize, uint32_t offset)
{
    // Serial.printf("read: lba:%d, bufsize:%d, offset: %d\n", lba, bufsize, offset);
    const sfud_flash *_flash = sfud_get_device_table() + 0;
    uint8_t result = sfud_read(_flash, lba * _flash->chip.erase_gran + offset, bufsize, (uint8_t *)buffer);
    if (result == SFUD_SUCCESS)
    {
        return bufsize;
    }
    return -1;
}

static int32_t msc_write_cb(uint32_t lba, uint8_t *buffer, uint32_t bufsize, uint32_t offset)
{
    // Serial.printf("write: lba:%d, bufsize:%d, offset:%d\n", lba, bufsize, offset);
    const sfud_flash *_flash = sfud_get_device_table() + 0;
    uint8_t result = SFUD_SUCCESS;
    if (offset == 0)
    {
        result = sfud_erase_write(_flash, lba * _flash->chip.erase_gran, bufsize, buffer);
    }
    else
    {
        result = sfud_write(_flash, lba * _flash->chip.erase_gran + offset, bufsize, buffer);
    }
    if (result == SFUD_SUCCESS)
    {
        return bufsize;
    }
    return -1;
}

static void msc_flush_cb(void)
{
}

CC::CC(/* args */)
{
    sd_mount = false;
    flash_mount = false;
}

CC::~CC()
{
}

void CC::begin()
{
    // 挂载文件系统
    flash_mount = SFUD.begin();

    // 如果flash挂载成功 则映射改空间
    if (flash_mount)
    {
        const sfud_flash *_flash = sfud_get_device_table() + 0;

        // Set disk vendor id, product id and revision with string up to 8, 16, 4 characters respectively
        usb_msc.setID("K1101", "Mass Storage", "1.0");

        // Set disk size
        usb_msc.setCapacity((_flash->chip.capacity / _flash->chip.erase_gran), _flash->chip.erase_gran);

        // Set callback
        usb_msc.setReadWriteCallback(msc_read_cb, msc_write_cb, msc_flush_cb);

        // Set Lun ready (RAM disk is always ready)
        usb_msc.setUnitReady(true);

        usb_msc.begin();
    }

    sd_mount = SD.begin(SDCARD_SS_PIN, SDCARD_SPI, 4000000UL);

    Serial.begin(115200);

    WiFi.mode(WIFI_MODE_AP);
}

String CC::SSID()
{
    if (flash_mount)
    {
        File config = SFUD.open("config.txt", "r");
        if (!config)
        {
            return String();
        }
        config.close();
        return config.readStringUntil('\n');
    }
    else
    {
        return String();
    }
}
String CC::PWD()
{
    // if (!File)
    //     return String();
}

CC sys;