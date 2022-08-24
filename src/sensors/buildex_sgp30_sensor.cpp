#include "buildex_sgp30_sensor.h"

buildex_sgp30_sensor::buildex_sgp30_sensor() {
}
void buildex_sgp30_sensor::init() {
    softwarei2c.begin(SGP30_SDAPIN, SGP30_SCLPIN);
    // Initialize sensor
    if (mySensor.begin(softwarei2c) == false) {
        return;
    }
    mySensor.initAirQuality();
}

bool buildex_sgp30_sensor::read(struct sensor_data *sdata) {

    SGP30ERR  err = SGP30_SUCCESS;
    err = mySensor.measureAirQuality();; // sgp_measure_iaq_blocking_read(&tvoc_ppb, &co2_eq_ppm);
    if (err == SGP30_SUCCESS) {
        sgp30_value[0] = mySensor.CO2;
        sgp30_value[1] = mySensor.TVOC;
    } else {
        return false;
    }

    sdata->data   = &sgp30_value;
    sdata->size   = sizeof(sgp30_value);
    sdata->id     = BUILDEX_SGP30;
    sdata->name   = name;
    sdata->status = true;
    return true;
}

const char *buildex_sgp30_sensor::get_name() {
    return "sgp30";
}

// buildex_sgp30_sensor  buildin_sgp30;       // Serial.println("Measurement failed!");