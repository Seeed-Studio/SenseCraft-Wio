#include "Seeed_sdcard_hal.h"
#include "Arduino.h"
#include "SPI.h"



typedef enum
{
    GO_IDLE_STATE = 0,
    SEND_OP_COND = 1,
    SEND_CID = 2,
    SEND_RELATIVE_ADDR = 3,
    SEND_SWITCH_FUNC = 6,
    SEND_IF_COND = 8,
    SEND_CSD = 9,
    STOP_TRANSMISSION = 12,
    SEND_STATUS = 13,
    SET_BLOCKLEN = 16,
    READ_BLOCK_SINGLE = 17,
    READ_BLOCK_MULTIPLE = 18,
    SEND_NUM_WR_BLOCKS = 22,
    SET_WR_BLK_ERASE_COUNT = 23,
    WRITE_BLOCK_SINGLE = 24,
    WRITE_BLOCK_MULTIPLE = 25,
    APP_OP_COND = 41,
    APP_CLR_CARD_DETECT = 42,
    APP_CMD = 55,
    READ_OCR = 58,
    CRC_ON_OFF = 59
} ardu_sdcard_command_t;

static ardu_sdcard_t *s_cards[_VOLUMES] = {NULL, NULL};

namespace
{

    struct AcquireSPI
    {
        ardu_sdcard_t *card;
        explicit AcquireSPI(ardu_sdcard_t *card)
            : card(card)
        {
            card->spi->beginTransaction(SPISettings(card->frequency, MSBFIRST, SPI_MODE0));
        }
        AcquireSPI(ardu_sdcard_t *card, int frequency)
            : card(card)
        {
            card->spi->beginTransaction(SPISettings(card->frequency, MSBFIRST, SPI_MODE0));
        }
        ~AcquireSPI()
        {
            card->spi->endTransaction();
        }

    private:
        AcquireSPI(AcquireSPI const &);
        AcquireSPI &operator=(AcquireSPI const &);
    };

} // namespace

/*
    SD SPI
 * */

bool sdWait(uint8_t pdrv, int timeout)
{
    char resp;
    uint32_t start = millis();

    do
    {
        resp = s_cards[pdrv]->spi->transfer(0xFF);
    } while (resp == 0x00 && (millis() - start) < (unsigned int)timeout);

    return (resp > 0x00);
}

void sdStop(uint8_t pdrv)
{
    s_cards[pdrv]->spi->transfer(0xFD);
}

void sdDeselectCard(uint8_t pdrv)
{
    ardu_sdcard_t *card = s_cards[pdrv];
    digitalWrite(card->ssPin, HIGH);
}

bool sdSelectCard(uint8_t pdrv)
{
    ardu_sdcard_t *card = s_cards[pdrv];
    digitalWrite(card->ssPin, LOW);
    sdWait(pdrv, 300);
    return true;
}

bool sdReadBytes(uint8_t pdrv, char *buffer, int length)
{
    char token;
    unsigned short crc;
    ardu_sdcard_t *card = s_cards[pdrv];
    char *p = buffer;

    uint32_t start = millis();
    do
    {
        token = card->spi->transfer(0xFF);
    } while (token == 0xFF && (millis() - start) < 500);

    if (token != 0xFE)
    {
        return false;
    }

    for (; p < buffer + length; p++)
    {
        *p = card->spi->transfer(0xFF);
    }
    crc = card->spi->transfer16(0xFFFF);
    return (!card->supports_crc || crc == CRC16(buffer, length));
}

char sdWriteBytes(uint8_t pdrv, const char *buffer, char token)
{
    ardu_sdcard_t *card = s_cards[pdrv];
    unsigned short crc = (card->supports_crc) ? CRC16(buffer, 512) : 0xFFFF;
    if (!sdWait(pdrv, 500))
    {
        return false;
    }
    card->spi->transfer(token);
    card->spi->transfer((uint8_t *)buffer, 512);
    card->spi->transfer16(crc);
    return (card->spi->transfer(0xFF) & 0x1F);
}

char sdCommand(uint8_t pdrv, char cmd, unsigned int arg, unsigned int *resp)
{

    char token;
    ardu_sdcard_t *card = s_cards[pdrv];

    for (int f = 0; f < 3; f++)
    {
        if (cmd == SEND_NUM_WR_BLOCKS || cmd == SET_WR_BLK_ERASE_COUNT || cmd == APP_OP_COND || cmd == APP_CLR_CARD_DETECT)
        {
            token = sdCommand(pdrv, APP_CMD, 0, NULL);
            sdDeselectCard(pdrv);
            if (token > 1)
            {
                return token;
            }
            if (!sdSelectCard(pdrv))
            {
                return 0xFF;
            }
        }

        char cmdPacket[7];
        cmdPacket[0] = cmd | 0x40;
        cmdPacket[1] = arg >> 24;
        cmdPacket[2] = arg >> 16;
        cmdPacket[3] = arg >> 8;
        cmdPacket[4] = arg;
        if (card->supports_crc || cmd == GO_IDLE_STATE || cmd == SEND_IF_COND)
        {
            cmdPacket[5] = (CRC7(cmdPacket, 5) << 1) | 0x01;
        }
        else
        {
            cmdPacket[5] = 0x01;
        }
        cmdPacket[6] = 0xFF;

        card->spi->transfer((uint8_t *)cmdPacket, (cmd == STOP_TRANSMISSION) ? 7 : 6);

        for (int i = 0; i < 9; i++)
        {
            token = card->spi->transfer(0xFF);
            if (!(token & 0x80))
            {
                break;
            }
        }

        if (token == 0xFF)
        {
            sdDeselectCard(pdrv);
            delay(100);
            sdSelectCard(pdrv);
            continue;
        }
        else if (token & 0x08)
        {
            sdDeselectCard(pdrv);
            delay(100);
            sdSelectCard(pdrv);
            continue;
        }
        else if (token > 1)
        {
            break;
        }

        if (cmd == SEND_STATUS && resp)
        {
            *resp = card->spi->transfer(0xFF);
        }
        else if ((cmd == SEND_IF_COND || cmd == READ_OCR) && resp)
        {
            for (uint8_t i = 0; i < 4; i++)
            {
                uint8_t temp = card->spi->transfer(0xFF);
                *resp = *resp * 256 + temp;
            }
        }
        break;
    }
    return token;
}

/*
    SPI SDCARD Communication
 * */

char sdTransaction(uint8_t pdrv, char cmd, unsigned int arg, unsigned int *resp)
{
    if (!sdSelectCard(pdrv))
    {
        return 0xFF;
    }
    char token = sdCommand(pdrv, cmd, arg, resp);
    sdDeselectCard(pdrv);
    return token;
}

bool sdReadSector(uint8_t pdrv, char *buffer, unsigned long long sector)
{
    for (int f = 0; f < 3; f++)
    {
        if (!sdSelectCard(pdrv))
        {
            break;
        }
        if (!sdCommand(pdrv, READ_BLOCK_SINGLE, (s_cards[pdrv]->type == CARD_SDHC) ? sector : sector << 9, NULL))
        {
            bool success = sdReadBytes(pdrv, buffer, 512);
            sdDeselectCard(pdrv);
            if (success)
            {
                return true;
            }
        }
        else
        {
            break;
        }
    }
    sdDeselectCard(pdrv);
    return false;
}

bool sdReadSectors(uint8_t pdrv, char *buffer, unsigned long long sector, int count)
{
    for (int f = 0; f < 3;)
    {
        if (!sdSelectCard(pdrv))
        {
            break;
        }

        if (!sdCommand(pdrv, READ_BLOCK_MULTIPLE, (s_cards[pdrv]->type == CARD_SDHC) ? sector : sector << 9, NULL))
        {
            do
            {
                if (!sdReadBytes(pdrv, buffer, 512))
                {
                    f++;
                    break;
                }

                sector++;
                buffer += 512;
                f = 0;
            } while (--count);

            if (sdCommand(pdrv, STOP_TRANSMISSION, 0, NULL))
            {
                break;
            }

            sdDeselectCard(pdrv);
            if (count == 0)
            {
                return true;
            }
        }
        else
        {
            break;
        }
    }
    sdDeselectCard(pdrv);
    return false;
}

bool sdWriteSector(uint8_t pdrv, const char *buffer, unsigned long long sector)
{
    for (int f = 0; f < 3; f++)
    {
        if (!sdSelectCard(pdrv))
        {
            break;
        }
        if (!sdCommand(pdrv, WRITE_BLOCK_SINGLE, (s_cards[pdrv]->type == CARD_SDHC) ? sector : sector << 9, NULL))
        {

            char token = sdWriteBytes(pdrv, buffer, 0xFE);
            sdDeselectCard(pdrv);

            if (token == 0x0A)
            {
                continue;
            }
            else if (token == 0x0C)
            {
                return false;
            }

            unsigned int resp;
            if (sdTransaction(pdrv, SEND_STATUS, 0, &resp) || resp)
            {
                return false;
            }
            return true;
        }
        else
        {
            break;
        }
    }
    sdDeselectCard(pdrv);
    return false;
}

bool sdWriteSectors(uint8_t pdrv, const char *buffer, unsigned long long sector, int count)
{
    char token;
    const char *currentBuffer = buffer;
    unsigned long long currentSector = sector;
    int currentCount = count;
    ardu_sdcard_t *card = s_cards[pdrv];

    for (int f = 0; f < 3;)
    {
        if (card->type != CARD_MMC)
        {
            if (sdTransaction(pdrv, SET_WR_BLK_ERASE_COUNT, currentCount, NULL))
            {
                break;
            }
        }

        if (!sdSelectCard(pdrv))
        {
            break;
        }

        if (!sdCommand(pdrv, WRITE_BLOCK_MULTIPLE, (card->type == CARD_SDHC) ? currentSector : currentSector << 9, NULL))
        {
            do
            {
                token = sdWriteBytes(pdrv, currentBuffer, 0xFC);
                if (token != 0x05)
                {
                    f++;
                    break;
                }
                currentBuffer += 512;
                f = 0;
            } while (--currentCount);

            if (!sdWait(pdrv, 500))
            {
                break;
            }

            if (currentCount == 0)
            {
                sdStop(pdrv);
                sdDeselectCard(pdrv);

                unsigned int resp;
                if (sdTransaction(pdrv, SEND_STATUS, 0, &resp) || resp)
                {
                    return false;
                }
                return true;
            }
            else
            {
                if (sdCommand(pdrv, STOP_TRANSMISSION, 0, NULL))
                {
                    break;
                }

                sdDeselectCard(pdrv);

                if (token == 0x0A)
                {
                    unsigned int writtenBlocks = 0;
                    if (card->type != CARD_MMC && sdSelectCard(pdrv))
                    {
                        if (!sdCommand(pdrv, SEND_NUM_WR_BLOCKS, 0, NULL))
                        {
                            char acmdData[4];
                            if (sdReadBytes(pdrv, acmdData, 4))
                            {
                                writtenBlocks = acmdData[0] << 24;
                                writtenBlocks |= acmdData[1] << 16;
                                writtenBlocks |= acmdData[2] << 8;
                                writtenBlocks |= acmdData[3];
                            }
                        }
                        sdDeselectCard(pdrv);
                    }
                    currentBuffer = buffer + (writtenBlocks << 9);
                    currentSector = sector + writtenBlocks;
                    currentCount = count - writtenBlocks;
                    continue;
                }
                else
                {
                    return false;
                }
            }
        }
        else
        {
            break;
        }
    }
    sdDeselectCard(pdrv);
    return false;
}

unsigned long sdGetSectorsCount(uint8_t pdrv)
{
    for (int f = 0; f < 3; f++)
    {
        if (!sdSelectCard(pdrv))
        {
            break;
        }

        if (!sdCommand(pdrv, SEND_CSD, 0, NULL))
        {
            char csd[16];
            bool success = sdReadBytes(pdrv, csd, 16);
            sdDeselectCard(pdrv);
            if (success)
            {
                if ((csd[0] >> 6) == 0x01)
                {
                    unsigned long size = (((unsigned long)(csd[7] & 0x3F) << 16) | ((unsigned long)csd[8] << 8) | csd[9]) + 1;
                    return size << 10;
                }
                unsigned long size = (((unsigned long)(csd[6] & 0x03) << 10) | ((unsigned long)csd[7] << 2) | ((csd[8] & 0xC0) >> 6)) + 1;
                size <<= ((
                              ((csd[9] & 0x03) << 1) | ((csd[10] & 0x80) >> 7)) +
                          2);
                size <<= (csd[5] & 0x0F);
                return size >> 9;
            }
        }
        else
        {
            break;
        }
    }

    sdDeselectCard(pdrv);
    return 0;
}

/*
    FATFS API
 * */

DSTATUS sd_disk_initialize(uint8_t pdrv)
{
    char token;
    unsigned int resp;
    unsigned int start;
    ardu_sdcard_t *card = s_cards[pdrv];
    if (!(card->status & STA_NOINIT))
    {
        return card->status;
    }

    AcquireSPI card_locked(card, 400000);

    digitalWrite(card->ssPin, HIGH);
    for (uint8_t i = 0; i < 20; i++)
    {
        card->spi->transfer(0XFF);
    }

    if (sdTransaction(pdrv, GO_IDLE_STATE, 0, NULL) != 1)
    {
        goto unknown_card;
    }

    token = sdTransaction(pdrv, CRC_ON_OFF, 0, NULL);
    if (token == 0x5)
    {
        card->supports_crc = false;
    }
    else if (token != 1)
    {
        goto unknown_card;
    }
    card->supports_crc = false;

    if (sdTransaction(pdrv, SEND_IF_COND, 0x1AA, &resp) == 1)
    {
        if ((resp & 0xFFF) != 0x1AA)
        {
            goto unknown_card;
        }

        if (sdTransaction(pdrv, READ_OCR, 0, &resp) != 1 || !(resp & (1 << 20)))
        {
            goto unknown_card;
        }

        start = millis();
        do
        {
            token = sdTransaction(pdrv, APP_OP_COND, 0x40000000, NULL);
        } while (token == 1 && (millis() - start) < 1000);

        if (token)
        {
            goto unknown_card;
        }

        if (!sdTransaction(pdrv, READ_OCR, 0, &resp))
        {
            if (resp & (1 << 30))
            {
                card->type = CARD_SDHC;
            }
            else
            {
                card->type = CARD_SD;
            }
        }
        else
        {
            goto unknown_card;
        }
    }
    else
    {
        if (sdTransaction(pdrv, READ_OCR, 0, &resp) != 1 || !(resp & (1 << 20)))
        {
            goto unknown_card;
        }

        start = millis();
        do
        {
            token = sdTransaction(pdrv, APP_OP_COND, 0x100000, NULL);
        } while (token == 0x01 && (millis() - start) < 1000);

        if (!token)
        {
            card->type = CARD_SD;
        }
        else
        {
            start = millis();
            do
            {
                token = sdTransaction(pdrv, SEND_OP_COND, 0x100000, NULL);
            } while (token != 0x00 && (millis() - start) < 1000);

            if (token == 0x00)
            {
                card->type = CARD_MMC;
            }
            else
            {
                goto unknown_card;
            }
        }
    }

    if (card->type != CARD_MMC)
    {
        if (sdTransaction(pdrv, APP_CLR_CARD_DETECT, 0, NULL))
        {
            goto unknown_card;
        }
    }

    if (card->type != CARD_SDHC)
    {
        if (sdTransaction(pdrv, SET_BLOCKLEN, 512, NULL) != 0x00)
        {
            goto unknown_card;
        }
    }

    card->sectors = sdGetSectorsCount(pdrv);

    if (card->frequency > 25000000)
    {
        card->frequency = 25000000;
    }

    card->status &= ~STA_NOINIT;
    return card->status;

unknown_card:
    card->type = CARD_UNKNOWN;
    return card->status;
}

DSTATUS sd_disk_status(uint8_t pdrv)
{
    return s_cards[pdrv]->status;
}

DRESULT sd_disk_read(uint8_t pdrv, uint8_t *buffer, DWORD sector, UINT count)
{
    ardu_sdcard_t *card = s_cards[pdrv];
    if (card->status & STA_NOINIT)
    {
        return RES_NOTRDY;
    }
    DRESULT res = RES_OK;

    AcquireSPI lock(card);

    if (count > 1)
    {
        res = sdReadSectors(pdrv, (char *)buffer, sector, count) ? RES_OK : RES_ERROR;
    }
    else
    {
        res = sdReadSector(pdrv, (char *)buffer, sector) ? RES_OK : RES_ERROR;
    }
    return res;
}

DRESULT sd_disk_write(uint8_t pdrv, const uint8_t *buffer, DWORD sector, UINT count)
{
    ardu_sdcard_t *card = s_cards[pdrv];
    if (card->status & STA_NOINIT)
    {
        return RES_NOTRDY;
    }

    if (card->status & STA_PROTECT)
    {
        return RES_WRPRT;
    }
    DRESULT res = RES_OK;

    AcquireSPI lock(card);

    if (count > 1)
    {
        res = sdWriteSectors(pdrv, (const char *)buffer, sector, count) ? RES_OK : RES_ERROR;
    }
    else
    {
        res = sdWriteSector(pdrv, (const char *)buffer, sector) ? RES_OK : RES_ERROR;
    }
    return res;
}

DRESULT sd_disk_ioctl(uint8_t pdrv, uint8_t cmd, void *buff)
{
    switch (cmd)
    {
    case CTRL_SYNC:
    {
        AcquireSPI lock(s_cards[pdrv]);
        if (sdSelectCard(pdrv))
        {
            sdDeselectCard(pdrv);
            return RES_OK;
        }
    }
        return RES_ERROR;
    case GET_SECTOR_COUNT:
        *((unsigned long *)buff) = s_cards[pdrv]->sectors;
        return RES_OK;
    case GET_SECTOR_SIZE:
        *((WORD *)buff) = 512;
        return RES_OK;
    case GET_BLOCK_SIZE:
        *((uint32_t *)buff) = 1;
        return RES_OK;
    }
    return RES_PARERR;
}

/*
    Public methods
 * */

uint8_t sdcard_uninit(uint8_t pdrv)
{
    ardu_sdcard_t *card = s_cards[pdrv];
    if (pdrv >= _VOLUMES || card == NULL)
    {
        return 1;
    }
    ff_diskio_register(pdrv, NULL);
    s_cards[pdrv] = NULL;
    free(card);

    return 0;
}

uint8_t sdcard_init(uint8_t cs, SPIClass *spi, int hz)
{

    uint8_t pdrv = 0xFF;
    if (ff_diskio_get_drive(&pdrv) != 0 || pdrv == 0xFF)
    {
        return pdrv;
    }

    ardu_sdcard_t *card = (ardu_sdcard_t *)malloc(sizeof(ardu_sdcard_t));
    if (!card)
    {
        return 0xFF;
    }

    card->frequency = hz;
    card->spi = spi;
    card->ssPin = cs;

    card->supports_crc = true;
    card->type = CARD_NONE;
    card->status = STA_NOINIT;

    pinMode(card->ssPin, OUTPUT);
    digitalWrite(card->ssPin, HIGH);

    s_cards[pdrv] = card;

    static const ff_diskio_impl_t sd_impl = {
        .init = &sd_disk_initialize,
        .status = &sd_disk_status,
        .read = &sd_disk_read,
        .write = &sd_disk_write,
        .ioctl = &sd_disk_ioctl};
    ff_diskio_register(pdrv, &sd_impl);

    return pdrv;
}

uint8_t sdcard_unmount(uint8_t pdrv)
{
    ardu_sdcard_t *card = s_cards[pdrv];
    if (pdrv >= _VOLUMES || card == NULL)
    {
        return 1;
    }
    card->status |= STA_NOINIT;
    card->type = CARD_NONE;

    TCHAR drv[3] = {_T(char('0' + pdrv)), _T(':'), _T('0')};

    f_mount(NULL, drv, 0);
    return 0;
}

bool sdcard_mount(uint8_t pdrv)
{
    ardu_sdcard_t *card = s_cards[pdrv];
    if (pdrv >= _VOLUMES || card == NULL)
    {

        return false;
    }

    FATFS fs;
    TCHAR drv[3] = {_T(char('0' + pdrv)), _T(':'), _T('0')};
    FRESULT res = f_mount(&fs, drv, 1);
    if (res != FR_OK)
    {
        return false;
    }
    AcquireSPI lock(card);
    return true;
}

uint32_t sdcard_num_sectors(uint8_t pdrv)
{
    ardu_sdcard_t *card = s_cards[pdrv];
    if (pdrv >= _VOLUMES || card == NULL)
    {
        return 0;
    }
    return card->sectors;
}

uint32_t sdcard_sector_size(uint8_t pdrv)
{
    if (pdrv >= _VOLUMES || s_cards[pdrv] == NULL)
    {
        return 0;
    }
    return 512;
}

sdcard_type_t sdcard_type(uint8_t pdrv)
{
    ardu_sdcard_t *card = s_cards[pdrv];
    if (pdrv >= _VOLUMES || card == NULL)
    {
        return CARD_NONE;
    }
    return card->type;
}