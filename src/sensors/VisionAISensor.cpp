#include "VisionAISensor.h"
#include "SensorsUtils.h"

GroveAI ai(Wire);

Vision::Vision() : Thread("VisionAI", 128, 1) {
}

void Vision::Run() {
    while (true) {
        if (ai.invoke()) // begin invoke
        {
            while (1) // wait for invoking finished
            {
                CMD_STATE_T ret = ai.state();
                if (ret == CMD_STATE_IDLE) {
                    break;
                }
                Delay(Ticks::MsToTicks(20));
            }
            uint8_t len = ai.get_result_len(); // receive how many people detect
            if (len) {
                object_detection_t val; // get data
                for (int i = 0, j = 0; i < len; i++, j += 2) {
                    ai.get_result(i, (uint8_t *)&val, sizeof(object_detection_t)); // get result
                    int conf    = val.confidence;
                    data[j]     = 1;
                    data[j + 1] = conf;
                }
            }
        }
    }
}

VisionSensor::VisionSensor() {
    vision = new Vision();
}

void VisionSensor::init() {
    vision->Start();

    if (ai.begin(ALGO_OBJECT_DETECTION, MODEL_EXT_INDEX_1)) {
        status = true;
    } else {
        status = false;
    }
}

bool VisionSensor::read(struct sensor_data *sdata) {
    sdata->id   = VISIONAISENSOR;
    sdata->name = name;

    if (status) {
        if (!ai.begin(ALGO_OBJECT_DETECTION, MODEL_EXT_INDEX_1)) {
            status = false;
            return false;
        }

        sdata->data   = vision->data;
        sdata->size   = vision->dsize * sizeof(int);
        sdata->status = vision->status;
        return true;
    } else {
        sdata->data   = NULL;
        sdata->status = false;
        init(); // init again, try to find the sensor
        return false;
    }
}

const char *VisionSensor::get_name() {
    return this->name;
}
