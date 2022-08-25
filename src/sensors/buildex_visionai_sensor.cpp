#include "buildex_visionai_sensor.h"

buildex_visionai_sensor::buildex_visionai_sensor() {
    state = 0;
}
void buildex_visionai_sensor::init() {
    softwarei2c.begin(VISIONAI_SDAPIN, VISIONAI_SCLPIN);
    ai.begin(ALGO_OBJECT_DETECTION, MODEL_EXT_INDEX_1); // Object detection and externel model 1
    state = 1;
}

bool buildex_visionai_sensor::read(struct sensor_data *sdata) {
    uint16_t temp = 0;
    softwarei2c.begin(VISIONAI_SDAPIN, VISIONAI_SCLPIN);

    if (ai.invoke()) // begin invoke
    {
        while (1) // wait for invoking finished
        {
            CMD_STATE_T ret = ai.state();
            if (ret == CMD_STATE_IDLE) {
                break;
            } else if (ret == CMD_STATE_ERROR) {
                return false;
            }
            delay(100);
        }
        uint8_t len = ai.get_result_len(); // receive how many people detect
        if (len) {
            object_detection_t data; // get data
            for (int i = 0; i < len; i++) {
                ai.get_result(i, (uint8_t *)&data, sizeof(object_detection_t)); // get result
                temp += data.confidence;
            }
            visionai_value[0] = len;
            visionai_value[1] = temp / len;
        } else {
            visionai_value[0] = 0x0;
            visionai_value[1] = 0x0;
        }
    } else {
        return false;
    }
    sdata->size = sizeof(visionai_value);
    sdata->data   = &visionai_value;
    sdata->id     = BUILDEX_VISIONAI;
    sdata->name   = name;
    sdata->status = true;
    return true;
}

const char *buildex_visionai_sensor::get_name() {
    return "visionai";
}

// buildex_visionai_sensor  buildin_visionai;