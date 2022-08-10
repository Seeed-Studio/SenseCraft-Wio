
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

/**
 * In case of no connection, Grove E5 will be detected every 5 seconds to see if there is a
 * connection, and will be connected to the network immediately after a successful connection.
 *
 * In case of continuous unsuccessful connection, the network will be connected once every 10
 * minutes
 */

LoRaThread::LoRaThread(SysConfig &config) : Thread("LoRaThread", 256, 1), cfg(config) {
    Start();
    Serial.begin(9600);
    lorae5 = new Disk91_LoRaE5(&Serial);
};

void LoRaThread::Init() {
    Serial1.begin(9600);
    if (!lorae5->begin(DSKLORAE5_HWSERIAL_CUSTOM, &Serial1)) {
        is_e5_init = false;
        LOGSS.println("LoRa E5 Init Failed");
        return;
    }

    frequency = cfg.lora_frequency.toInt();
    if (frequency <= 0 || frequency > 10) {
        return;
    }

    // Setup the LoRaWan Credentials
    if (!lorae5->setup_sensecap(frequency, false,
                                true)) { // Setup the LoRaWAN stack with the stored credentials
        is_e5_init = false;
        LOGSS.println("LoRa E5 Setup Sensecap Failed");
        return;
    }

    LOGSS.println("LoRa E5 Init OK");
    is_e5_init = true;

    // join network immediately
    Join();
}

void LoRaThread::Join() {
    if (!lorae5->join_sync()) {
        is_e5_join = false;
        LOGSS.println("LoRa E5 Join Failed");
    } else {
        LOGSS.println("LoRa E5 Join Success");
        is_e5_join = true;
        // Send Device Info first lorawan message
        SendDeviceInfo();
    }
}

// Every 10s, 10 failed attempts to send data and break the connection, re-join the network
bool LoRaThread::SendData(uint8_t *data, uint8_t len, uint8_t ver) {
    bool    ret   = true;
    uint8_t retry = 0;
    while (!lorae5->sendReceive_sync(ver, data, len, downlink_rxBuff, &downlink_rxSize,
                                     &downlink_rxPort, LORAE5_SF7, LORAE5_14DB, LORAE5_RETRY_3)) {
        Delay(Ticks::SecondsToTicks(10));
        if (retry++ > 10) {
            is_e5_join = false;
            ret        = false;
            break;
        }
    }
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
                sdata.y = BSWAP16(static_cast<uint16_t>(((int32_t *)data.data)[4]));
                sdata.z = BSWAP16(static_cast<uint16_t>(((int32_t *)data.data)[8]));
                LOGSS.printf("X: %d, Y: %d, Z: %d\r\n", sdata.x, sdata.y, sdata.z);
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

bool LoRaThread::SendGroveSensorData() {
    struct grove_sensor_data sdata;
    bool                     ret = true;
    sdata.precode                = 0x42;
    /*build sensor data*/
    sdata.d0 = 0x80;
    sdata.d1 = 0x80;
    sdata.d2 = 0x80;
    sdata.d3 = 0x80;
    sdata.d4 = 0x80;
    if (!SendData((uint8_t *)&sdata, sizeof(sdata), v2)) {
        LOGSS.println("LoRa E5 Send Buildin Sensor Data Failed");
        ret = false;
    }
    return ret;
}

void LoRaThread::Run() {
    // init the library, search the LORAE5 over the different WIO port available
    Serial.begin(9600);
    while (!Serial) {
        Delay(Ticks::MsToTicks(1));
    }
    Serial.println("LoRa E5 Run..");
    lorae5 = new Disk91_LoRaE5(&Serial);
    Init();
    while (true) {
        if (!is_e5_init) {
            // try to init the LoRa E5 5s after the last failure
            Delay(Ticks::SecondsToTicks(5));
            Init();
            continue;
        }

        if (!is_e5_join) {
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

        // clear all data in the lora_data queue
        lora_data.clear();
        lora_data_ready = true;
        Delay(Ticks::SecondsToTicks(60 * 5));
    }
}

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