#include "buildin_mic.h"
#include "Arduino.h"

buildin_mic::buildin_mic() {
}
void buildin_mic::init() {
    pinMode(WIO_MIC, INPUT);
}
bool buildin_mic::read(struct sensor_data *sdata) {
    mic_value        = analogRead(WIO_MIC);
    sdata->data_type = SENSOR_DATA_TYPE_INT32;
    sdata->data      = &mic_value;
    sdata->size      = sizeof(mic_value);
    sdata->id        = BUILDIN_MIC;
    sdata->name      = name;
    sdata->ui_type   = SENSOR_UI_TYPE_NORMAL;
    sdata->data_unit = data_unit;
    sdata->status    = true;
    return true;
}

const char *buildin_mic::get_name() {
    return "buildin-mic";
}
