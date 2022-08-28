#include "grove_visionai_sensor.h"
#include "Arduino.h"
#include "SensorsUtils.h"

Visionai::Visionai() : Thread("Visionai", 512, 1) {
}

void Visionai::pushlog(const char *msg) {
    log_data log;
    strcpy(log.data, msg);
    log.time = millis();
    if (ai_log.size() > LOG_MAX_SIZE) 
    {
        ai_log.erase(ai_log.begin()); 
    }
    ai_log.push_back(log);
}

void Visionai::Run() {
    uint16_t temp = 0;
    char     log[32];
    softwarei2c.begin(VISIONAI_SDAPIN, VISIONAI_SCLPIN);
    ai.begin(ALGO_OBJECT_DETECTION, MODEL_EXT_INDEX_1); // Object detection and externel model 1
    while (true) {
        softwarei2c.begin(VISIONAI_SDAPIN, VISIONAI_SCLPIN);
        temp = 0;
        uint32_t tick = millis();
        if (ai.invoke()) // begin invoke
        {
            while (1) // wait for invoking finished
            {
                CMD_STATE_T ret = ai.state();
                if (ret == CMD_STATE_IDLE) {
                    break;
                } else if (ret == CMD_STATE_ERROR) {
                    pushlog("Vision Ai is disconnect.");
                    status = false;
                    goto next;
                }
                Delay(Ticks::MsToTicks(100));
            }
            uint8_t len = ai.get_result_len(); // receive how many people detect
            if (len) {
                int time1 = millis() - tick;
                sprintf(log, "Time consuming: %d", time1);
                pushlog(log);
                sprintf(log, "Number of people: %d", len);
                pushlog(log);
                object_detection_t g_data; // get data
                for (int i = 0; i < len; i++) {
                    sprintf(log, "Detecting and calculating: %d", i + 1);
                    pushlog(log);
                    ai.get_result(i, (uint8_t *)&g_data, sizeof(object_detection_t)); // get result
                    temp += g_data.confidence;
                    sprintf(log, "confidence: %d", g_data.confidence);
                    pushlog(log);
                }
                status  = true;
                data[0] = len;
                data[1] = temp / len;
            } else {
                pushlog("No identification");
                status  = true;
                data[0] = 0x0;
                data[1] = 0x0;
            }
        } else {
            pushlog("Vision Ai is disconnect.");
            status = false;
        }
    next:
        Delay(Ticks::MsToTicks(SENSOR_READ_DELAY));
    }
}

grove_visionai_sensor::grove_visionai_sensor() {
    visionai = new Visionai();
}
void grove_visionai_sensor::init() {
    visionai->Start();
}

bool grove_visionai_sensor::read(struct sensor_data *sdata) {
    sdata->size                 = sizeof(visionai->data);
    sdata->data                 = &visionai->data;
    sdata->id                   = GROVE_VISIONAI;
    sdata->name                 = name;
    sdata->status               = visionai->status;
    grove_visionai_sensor::slog = visionai->ai_log;
    return sdata->status;
}

const char *grove_visionai_sensor::get_name() {
    return "visionai";
}

// grove_visionai_sensor  buildin_visionai;