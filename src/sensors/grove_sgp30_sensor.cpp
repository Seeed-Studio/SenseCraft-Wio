#include "grove_sgp30_sensor.h"

grove_sgp30_sensor::grove_sgp30_sensor() {
}
void grove_sgp30_sensor::init() {
    softwarei2c.begin(SGP30_SDAPIN, SGP30_SCLPIN);
    // Initialize sensor
    if (mySensor.begin(softwarei2c) == false) {
        return;
    }
    mySensor.initAirQuality();
}

bool grove_sgp30_sensor::read(struct sensor_data *sdata) {
    softwarei2c.begin(SGP30_SDAPIN, SGP30_SCLPIN);
    if (mySensor.measureAirQuality() == SGP30_SUCCESS) {
        sgp30_value[0] = mySensor.CO2;
        sgp30_value[1] = mySensor.TVOC;
    } else {
        return false;
    }

    sdata->data   = &sgp30_value;
    sdata->size   = sizeof(sgp30_value);
    sdata->id     = GROVE_SGP30;
    sdata->name   = name;
    sdata->status = true;
    return true;
}

const char *grove_sgp30_sensor::get_name() {
    return "sgp30";
}

// grove_sgp30_sensor  buildin_sgp30;       // Serial.println("Measurement failed!");