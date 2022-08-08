#include "system.h"

using namespace cpp_freertos;

SemaphoreHandle_t System::lock = NULL;

static int32_t msc_read_cb(uint32_t lba, void *buffer, uint32_t bufsize, uint32_t offset)
{
    // Serial.printf("read: lba:%d, bufsize:%d, offset: %d\n", lba, bufsize, offset);
    uint32_t ret_size = -1;

    xSemaphoreTake(System::lock, portMAX_DELAY);

    const sfud_flash *_flash = sfud_get_device_table() + 0;
    uint8_t result = sfud_read(_flash, lba * _flash->chip.erase_gran + offset, bufsize, (uint8_t *)buffer);
    if (result == SFUD_SUCCESS)
    {
        ret_size = bufsize;
    }
    xSemaphoreGive(System::lock);
    return ret_size;
}

static int32_t msc_write_cb(uint32_t lba, uint8_t *buffer, uint32_t bufsize, uint32_t offset)
{
    // Serial.printf("write: lba:%d, bufsize:%d, offset:%d\n", lba, bufsize, offset);
    uint32_t ret_size = -1;
    const sfud_flash *_flash = sfud_get_device_table() + 0;
    uint8_t result = SFUD_SUCCESS;
    xSemaphoreTake(System::lock, portMAX_DELAY);
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
        ret_size = bufsize;
    }
    xSemaphoreGive(System::lock);
    return ret_size;
}

static void msc_flush_cb(void)
{
}

System::System(/* args */)
{
    sd_mount = false;
    flash_mount = false;
}

System::~System()
{
}

void System::begin()
{
    // 挂载文件系统
    if (System::lock == NULL)
    {
        System::lock = xSemaphoreCreateMutex();
    }
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
    pinMode(SDCARD_DET_PIN, INPUT);
    if (digitalRead(SDCARD_DET_PIN) == LOW)
    {
        sd_mount = SD.begin(SDCARD_SS_PIN, SDCARD_SPI, 4000000UL);
    }

    Serial.begin(115200);

    WiFi.mode(WIFI_MODE_AP);
}

String System::SSID()
{
    String ssid;
    xSemaphoreTake(System::lock, portMAX_DELAY);
    if (flash_mount)
    {
        File config = SFUD.open("config.txt", "r");
        if (config)
        {
            if (-1 != config.find("SSID="))
            {
                ssid = config.readStringUntil('\n');
                ssid.trim();
            }
            config.close();
        }
    }
    xSemaphoreGive(System::lock);
    return ssid;
}
String System::PWD()
{
    String pwd;
    xSemaphoreTake(System::lock, portMAX_DELAY);
    if (flash_mount)
    {
        File config = SFUD.open("config.txt", "r");
        if (config)
        {

            if (-1 != config.find("PWD="))
            {
                pwd = config.readStringUntil('\n');
                pwd.trim();
            }
            config.close();
        }
    }
    xSemaphoreGive(System::lock);
    return pwd;
}

uint32_t System::write(uint8_t *buff, uint32_t size)
{

    if (!TF()) // tf 没接入
    {
        sd_mount = 0;
        return 0;
    }
    else // tf卡接入
    {
        if (!sd_mount) // tf卡没被挂载
        {
            sd_mount = SD.begin(SDCARD_SS_PIN, SDCARD_SPI, 4000000UL);
            if (!sd_mount)
            {
                return 0;
            }
        }
    }
    File data = SD.open("data.txt", "a");
    if (data)
    {
        data.write(buff, size);
        data.close();
    }
    return size;
}

bool System::TF()
{
    return digitalRead(SDCARD_DET_PIN) == LOW;
}

System sys;