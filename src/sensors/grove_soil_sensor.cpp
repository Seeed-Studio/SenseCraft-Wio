#include "grove_soil_sensor.h"

grove_soil_sensor::grove_soil_sensor() {
}
void grove_soil_sensor::init() {
}

bool grove_soil_sensor::read(struct sensor_data *sdata) {
    uint16_t sum = 0, data[READ_NUM] = {0};
    double   variance = 0.0;

    analogRead(SOILPIN);
    delay(2);
    // 求和
    for (int i = 0; i < READ_NUM; i++) {
        data[i] = analogRead(SOILPIN);
        delayMicroseconds(2);
        sum += data[i];
    }
    //求方差
    for (int i = 0; i < READ_NUM; i++) {
        variance = variance + pow(data[i] - sum / READ_NUM, 2);
    }
    variance = variance / READ_NUM;

    if (variance > 20 || sum / READ_NUM > 1000)
        return false;
    soil_value    = sum / READ_NUM;
    sdata->data   = &soil_value;
    sdata->size   = sizeof(soil_value);
    sdata->id     = GROVE_SOIL;
    sdata->name   = name;
    sdata->status = true;
    return true;
}

const char *grove_soil_sensor::get_name() {
    return "soil";
}

// grove_soil_sensor  buildin_soil;       // Serial.println("Measurement failed!");