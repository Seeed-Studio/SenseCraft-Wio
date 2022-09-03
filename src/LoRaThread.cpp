
#include "LoRaThread.h"

#pragma pack(1)

#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
#define BSWAP16(code) __builtin_bswap16(code);
#define BSWAP32(code) __builtin_bswap32(code);
#else //大端模式则什么也不做直接返回
#define BSWAP16(code) code
#define BSWAP32(code) code
#endif

enum LORAE5_SF {
    LORAE5_SF7  = 7,
    LORAE5_SF8  = 8,
    LORAE5_SF9  = 9,
    LORAE5_SF10 = 10,
    LORAE5_SF11 = 11,
    LORAE5_SF12 = 12,
};

enum LORAE5_PWR {
    LORAE5_10DB = 10,
    LORAE5_12DB = 12,
    LORAE5_14DB = 14,
    LORAE5_16DB = 16,
    LORAE5_18DB = 18,
    LORAE5_20DB = 20,
};

enum LORAE5_RETRY {
    LORAE5_RETRY_0 = 0,
    LORAE5_RETRY_1 = 1,
    LORAE5_RETRY_2 = 2,
    LORAE5_RETRY_3 = 3,
};

struct buildin_sensor_data {
    uint8_t  precode;
    uint16_t light;
    uint16_t mic;
    uint16_t x;
    uint16_t y;
    uint16_t z;
};

struct grove_sensor_data {
    uint8_t  precode;
    uint16_t d0;
    uint16_t d1;
    uint16_t d2;
    uint16_t d3;
    uint16_t d4;
};
#pragma pack(1)
struct grove_ai_vision_data {
    uint8_t  precode;
    uint16_t data[5];
};
#pragma pack()
/**
 * In case of no connection, Grove E5 will be detected every 5 seconds to see if there is a
 * connection, and will be connected to the network immediately after a successful connection.
 *
 * In case of continuous unsuccessful connection, the network will be connected once every 10
 * minutes
 */

LoRaThread::LoRaThread(SysConfig &config) : Thread("LoRaThread", 512, 1), cfg(config) {
    Start();
};

void LoRaThread::Init() {
    Serial3.begin(9600);
    if (!lorae5->begin(DSKLORAE5_HWSERIAL_CUSTOM, &Serial3)) {
        cfg.lora_status = LORA_INIT_FAILED;
        LOGSS.println("LoRa E5 Init Failed");
        return;
    }

    frequency = cfg.lora_frequency;
    if (frequency <= 0 || frequency > 10) {
        return;
    }

    // Setup the LoRaWan Credentials
    if (!lorae5->setup_sensecap(frequency, false,
                                true)) { // Setup the LoRaWAN stack with the stored credentials
        cfg.lora_status = LORA_INIT_FAILED;
        LOGSS.println("LoRa E5 Setup Sensecap Failed");
        return;
    }

    LOGSS.println("LoRa E5 Init OK");
    cfg.lora_status = LORA_INIT_SUCCESS;

    // join network immediately
    Join();
}

void LoRaThread::Join() {
    if (!lorae5->join_sync()) {
        cfg.lora_status = LORA_JOIN_FAILED;
        LOGSS.println("LoRa E5 Join Failed");
    } else {
        LOGSS.println("LoRa E5 Join Success");
        cfg.lora_status = LORA_JOIN_SUCCESS;
        // Send Device Info first lorawan message
        SendDeviceInfo();
    }
}

// Every 10s, 10 failed attempts to send data and break the connection, re-join the network
bool LoRaThread::SendData(uint8_t *data, uint8_t len, uint8_t ver) {
    bool    ret   = true;
    uint8_t retry = 0;
    cfg.lora_fcnt++;
    while (!lorae5->sendReceive_sync(ver, data, len, downlink_rxBuff, &downlink_rxSize,
                                     &downlink_rxPort, LORAE5_SF7, LORAE5_14DB, LORAE5_RETRY_3)) {
        cfg.lora_status = LORA_SEND_FAILED;
        cfg.lora_rssi   = -120;
        cfg.lora_fcnt++;
        Delay(Ticks::SecondsToTicks(10));
        if (retry++ > 10) {
            cfg.lora_status = LORA_JOIN_FAILED;
            cfg.lora_rssi   = 0;
            ret             = false;
            break;
        }
    }
    cfg.lora_status = LORA_SEND_SUCCESS;
    cfg.lora_rssi   = lorae5->getRssi();
    cfg.lora_sucess_cnt++;
    return ret;
}

bool LoRaThread::SendDeviceInfo() {
    // Electricity information
    uint8_t electricity_info[9] = {0x00, 0x07, 0x00, 0x64, 0x00, 0x05, 0x00, 0x61, 0x89};
    uint8_t version_info[9]     = {0x00, 0x00, 0x00, 0x01, 0x01, 0x02, 0x02, 0xC5, 0x56};

    // Send electricity information
    if (!SendData(electricity_info, 9, v1)) {
        LOGSS.println("LoRa E5 Send Electricity Info Failed");
        return false;
    }
    LOGSS.println("LoRa E5 Send Electricity Info Success");
    // Send version information
    if (!SendData(version_info, 9, v1)) {
        LOGSS.println("LoRa E5 Send Version Info Failed");
        return false;
    }
    LOGSS.println("LoRa E5 Send Version Info Success");
    return true;
}

bool LoRaThread::SendVisionAIInfo() {
    // Electricity information
    uint8_t VisionAI_info[9] = {0x01, 0x09, 0x00, 0x00, 0x00, 0x11, 0x00, 0xEF, 0x36};

    // Send electricity information
    if (!SendData(VisionAI_info, 9, v1)) {
        LOGSS.println("LoRa E5 Send VisionAI Info Failed");
        return false;
    }
    LOGSS.println("LoRa E5 Send VisionAI Info Success");
    return true;
}

bool LoRaThread::SendBuildinSensorData() {
    struct buildin_sensor_data sdata;
    sdata.precode = 0x40;
    bool ret      = true;
    LOGSS.println("====================================");
    LOGSS.println("LoRa E5 Send Buildin Sensor Data");
    // Send Buildin Sensor Data
    for (auto data : lora_data) {
        // Copy the data to the buildin buffer in the order of the data id
        switch (data.id) {

            {
            case BUILDIN_LIGHT:
                /* code */
                sdata.light = BSWAP16(static_cast<uint16_t>(((int32_t *)data.data)[0]));
                LOGSS.printf("Light: %d\r\n, ", sdata.light);
                break;
            case BUILDIN_MIC:
                sdata.mic = BSWAP16(static_cast<uint16_t>(((int32_t *)data.data)[0]));
                LOGSS.printf("Mic: %d\r\n, ", sdata.mic);
                /* code */
                break;
            case LIS3DHTRSENSOR:
                /* code */
                sdata.x = BSWAP16(static_cast<uint16_t>(((int32_t *)data.data)[0]));
                sdata.y = BSWAP16(static_cast<uint16_t>(((int32_t *)data.data)[1]));
                sdata.z = BSWAP16(static_cast<uint16_t>(((int32_t *)data.data)[2]));
                LOGSS.printf("X: %d, Y: %d, Z: %d\r\n", sdata.x, sdata.y, sdata.z);
                break;
            default:
                return ret;
                break;
            }
        }
    }
    LOGSS.printf("data size %d\r\n", sizeof(sdata));
    LOGSS.println("====================================");

    if (!SendData((uint8_t *)&sdata, sizeof(sdata), v2)) {
        LOGSS.println("LoRa E5 Send Buildin Sensor Data Failed");
        ret = false;
    }

    return ret;
}

bool LoRaThread::SendGroveSensorData() {
    struct grove_sensor_data sdata = {0};
    bool                     ret   = true;
    sdata.precode                  = 0x42;
    sdata.d0                       = 0x80;
    sdata.d1                       = 0x80;
    sdata.d2                       = 0x80;
    sdata.d3                       = 0x80;
    sdata.d4                       = 0x80;
    /*build sensor data*/
    for (auto data : lora_data) {
        // Copy the data to the buildin buffer in the order of the data id
        switch (data.id) {
            {
            case GROVE_SOIL:
                /* code */
                sdata.d4 = BSWAP16(static_cast<uint16_t>(((int32_t *)data.data)[0]));
                LOGSS.printf("Soil hum: %d\r\n, ", sdata.d4);
                break;
            case GROVE_SGP30:
                sdata.d3 = BSWAP16(static_cast<uint16_t>(((int32_t *)data.data)[0]));
                sdata.d2 = BSWAP16(static_cast<uint16_t>(((int32_t *)data.data)[1]));
                LOGSS.printf("CO2: %d ,TVOC:%d\r\n, ", sdata.d2, sdata.d3);
                /* code */
                break;
            case GROVE_SHT4X:
                /* code */
                sdata.d0 = BSWAP16(static_cast<uint16_t>(((int32_t *)data.data)[0]));
                sdata.d1 = BSWAP16(static_cast<uint16_t>(((int32_t *)data.data)[1]));
                LOGSS.printf("temperature: %d, humidity: %d\r\n", sdata.d0, sdata.d1);
                break;
            default:
                break;
            }
        }
    }
    LOGSS.printf("data size %d\r\n", sizeof(sdata));
    LOGSS.println("====================================");
    if (!SendData((uint8_t *)&sdata, sizeof(sdata), v2)) {
        LOGSS.println("LoRa E5 Send Buildin Sensor Data Failed");
        ret = false;
    }
    return ret;
}

bool LoRaThread::SendAiVisionData() {
    struct grove_ai_vision_data sdata = {0};
    bool                        ret   = true;
    sdata.precode                     = 0x43;
    sdata.data[0]                     = 0xffff;
    sdata.data[1]                     = 0xffff;
    sdata.data[2]                     = 0xffff;
    sdata.data[3]                     = 0xffff;
    sdata.data[4]                     = 0xffff;
    /*build sensor data*/
    for (auto data : lora_data) {
        // Copy the data to the buildin buffer in the order of the data id
        switch (data.id) {
            {
            case GROVE_VISIONAI:
                /* code */
                if (!SendVisionAIInfo()) {
                    // try to send the Ai Vision data 5 minutes  after the last failure
                    return false;
                }
                if (data.size / 4 < 5) {
                    sdata.precode = 0x44;
                    for (int i = 0; i < data.size / 4; i++) {
                        sdata.data[i] = BSWAP16(static_cast<uint16_t>(((int32_t *)data.data)[i]));
                    }
                    LOGSS.printf("data size %d\r\n", sizeof(sdata));
                    LOGSS.println("====================================");
                    if (!SendData((uint8_t *)&sdata, sizeof(sdata), v2)) {
                        LOGSS.println("LoRa E5 Send Buildin Sensor Data Failed");
                        ret = false;
                    }
                } else {
                    sdata.precode = 0x43;
                    for (int i = 0; i < 5; i++) {
                        sdata.data[i] = BSWAP16(static_cast<uint16_t>(((int32_t *)data.data)[i]));
                    }
                    LOGSS.printf("data size %d\r\n", sizeof(sdata));
                    LOGSS.println("====================================");
                    if (!SendData((uint8_t *)&sdata, sizeof(sdata), v2)) {
                        LOGSS.println("LoRa E5 Send Buildin Sensor Data Failed");
                        ret = false;
                    }
                    sdata.precode = 0x45;
                    for (int n = 5; n < (data.size / 4); n += 5) {
                        sdata.data[0] = 0xffff;
                        sdata.data[1] = 0xffff;
                        sdata.data[2] = 0xffff;
                        sdata.data[3] = 0xffff;
                        sdata.data[4] = 0xffff;
                        for (int i = 0; i < 5; i++) {
                            sdata.data[i] =
                                BSWAP16(static_cast<uint16_t>(((int32_t *)data.data)[n + i]));
                        }
                        LOGSS.printf("data size %d\r\n", sizeof(sdata));
                        LOGSS.println("====================================");
                        if (!SendData((uint8_t *)&sdata, sizeof(sdata), v2)) {
                            LOGSS.println("LoRa E5 Send Buildin Sensor Data Failed");
                            ret = false;
                        }
                    }
                }
                break;
            default:
                return ret;
                break;
            }
        }
    }
    return ret;
}

#if 1
void LoRaThread::Run() {
    // init the library, search the LORAE5 over the different WIO port available
    Serial.begin(9600);
    Serial.println("LoRa E5 Run..");
    lorae5 = new Disk91_LoRaE5(&Serial);
    // Init();
    while (true) {
        while (cfg.lora_on) {
            LOGSS.printf("LoRa Sensor number: %d  %d \r\n", lora_data.size(), lora_data.capacity());
            if (cfg.lora_status == LORA_INIT_FAILED) {
                // try to init the LoRa E5 5s after the last failure
                Delay(Ticks::SecondsToTicks(5));
                Init();
                continue;
            }

            if (cfg.lora_status == LORA_INIT_SUCCESS || cfg.lora_status == LORA_JOIN_FAILED) {
                // try to join the LoRa E5 5 minutes  after the last failure
                Delay(Ticks::SecondsToTicks(30));
                Join();
                continue;
            }

            if (!SendBuildinSensorData()) {
                // try to send the buildin sensor data 5 minutes  after the last failure
                Delay(Ticks::SecondsToTicks(60 * 5));
                continue;
            }
            if (!SendGroveSensorData()) {
                // try to send the grove sensor data 5 minutes  after the last failure
                Delay(Ticks::SecondsToTicks(60 * 5));
                continue;
            }
            if (!SendAiVisionData()) {
                // try to send the Ai Vision data 5 minutes  after the last failure
                Delay(Ticks::SecondsToTicks(60 * 5));
                continue;
            }
            // clear all data in the lora_data queue
            lora_data.clear();
            lora_data.shrink_to_fit();
            lora_data_ready = true;
            Delay(Ticks::SecondsToTicks(60 * 5));
        }
        // 暂时延时处理
        Delay(Ticks::SecondsToTicks(100));
    }
}
#elif

void LoRaThread::Run() {
    while (true) {
        Delay(Ticks::SecondsToTicks(60 * 5));
    }
}
#endif

// Store the received sensor data into a queue of length 30.
void LoRaThread::LoRaPushData(std::vector<sensor_data *> d) {
    // A loop to deep copy param of d vector into new lora_data queue
    // by Iterative method
    if (lora_data_ready) {
        for (auto data : d)
            lora_data.push_back(*data);

        lora_data_ready = false;
    }
}