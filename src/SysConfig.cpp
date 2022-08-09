#include "SysConfig.h"

using namespace cpp_freertos;

SemaphoreHandle_t SysConfig::lock = NULL;

static int32_t msc_read_cb(uint32_t lba, void *buffer, uint32_t bufsize, uint32_t offset) {
    // LOGSS.printf("read: lba:%d, bufsize:%d, offset: %d\n", lba, bufsize, offset);
    uint32_t ret_size = -1;

    xSemaphoreTake(SysConfig::lock, portMAX_DELAY);

    const sfud_flash *_flash = sfud_get_device_table() + 0;
    uint8_t           result =
        sfud_read(_flash, lba * _flash->chip.erase_gran + offset, bufsize, (uint8_t *)buffer);
    if (result == SFUD_SUCCESS) {
        ret_size = bufsize;
    }
    xSemaphoreGive(SysConfig::lock);
    return ret_size;
}

static int32_t msc_write_cb(uint32_t lba, uint8_t *buffer, uint32_t bufsize, uint32_t offset) {
    // LOGSS.printf("write: lba:%d, bufsize:%d, offset:%d\n", lba, bufsize, offset);
    uint32_t          ret_size = -1;
    const sfud_flash *_flash   = sfud_get_device_table() + 0;
    uint8_t           result   = SFUD_SUCCESS;
    xSemaphoreTake(SysConfig::lock, portMAX_DELAY);
    if (offset == 0) {
        result = sfud_erase_write(_flash, lba * _flash->chip.erase_gran, bufsize, buffer);
    } else {
        result = sfud_write(_flash, lba * _flash->chip.erase_gran + offset, bufsize, buffer);
    }
    if (result == SFUD_SUCCESS) {
        ret_size = bufsize;
    }
    xSemaphoreGive(SysConfig::lock);
    return ret_size;
}

static void msc_flush_cb(void) {
}

SysConfig::SysConfig(/* args */) {
}

SysConfig::~SysConfig() {
}

void SysConfig::init() {
    // 挂载文件系统
    if (SysConfig::lock == NULL) {
        SysConfig::lock = xSemaphoreCreateMutex();
    }
    spi_flash_mount = SFUD.begin();

    // 如果flash挂载成功 则映射改空间
    if (spi_flash_mount) {
        const sfud_flash *_flash = sfud_get_device_table() + 0;

        // Set disk vendor id, product id and revision with string up to 8, 16, 4 characters
        // respectively
        usb_msc.setID("K1101", "Mass Storage", "1.0");

        // Set disk size
        usb_msc.setCapacity((_flash->chip.capacity / _flash->chip.erase_gran),
                            _flash->chip.erase_gran);

        // Set callback
        usb_msc.setReadWriteCallback(msc_read_cb, msc_write_cb, msc_flush_cb);

        // Set Lun ready (RAM disk is always ready)
        usb_msc.setUnitReady(true);

        usb_msc.begin();
    } 
    // pinMode(SDCARD_DET_PIN, INPUT);
    // if (digitalRead(SDCARD_DET_PIN) == LOW) {
    //     sd_mount = SD.begin(SDCARD_SS_PIN, SDCARD_SPI, 4000000UL);
    // }else
    // 	LOGSS.println("sd card not insert");
    ReadAllConfig();
}

// void SysConfig::set_lora_freq(uint8_t frequency)
// {
// 	this->frequency = frequency;
// }

// uint8_t SysConfig::get_lora_freq()
// {
// 	return this->frequency;
// }

void SysConfig::ReadConfigParam(const String filename, char *prefix_param, String *param) {
    xSemaphoreTake(SysConfig::lock, portMAX_DELAY);
    if (spi_flash_mount) {
        File config = SFUD.open(filename, "r");
        if (config) {

            if (-1 != config.find(prefix_param)) {
                *param = config.readStringUntil('\n');
                param->trim();
            }
            config.close();
        }
    }
    xSemaphoreGive(SysConfig::lock);
}

void SysConfig::ReadAllConfig() {
    ReadConfigParam("config.txt", "SSID=", &this->ssid);
    ReadConfigParam("config.txt", "PASSWORD=", &this->password);
    ReadConfigParam("config.txt", "FREQUENCY=", &this->lora_frequency);
}

void SysConfig::WriteConfigParam(char *filename, char *prefix_param, char *param) {
    xSemaphoreTake(SysConfig::lock, portMAX_DELAY);
    if (spi_flash_mount) {
        File config = SFUD.open(filename, "r+");
        if (config) {
            if (-1 != config.find(prefix_param)) {
                config.seek(config.position());
                config.print(param);
                config.print("\n");
            }
        } else {
            config = SFUD.open(filename, "w");
            if (config) {
                config.print(prefix_param);
                config.print(param);
                config.print("\n");
            }
        }
        config.close();
    }
    xSemaphoreGive(SysConfig::lock);
}