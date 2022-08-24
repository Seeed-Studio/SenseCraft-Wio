#include "buildex_soil_sensor.h"

buildex_soil_sensor::buildex_soil_sensor() {
}
void buildex_soil_sensor::init() {

}

bool buildex_soil_sensor::read(struct sensor_data *sdata) {
    soil_value = analogRead(SOILPIN);
    sdata->data   = &soil_value;
    sdata->size   = sizeof(soil_value);
    sdata->id     = BUILDEX_SOIL;
    sdata->name   = name;
    sdata->status = true;
    return true;
}

const char *buildex_soil_sensor::get_name() {
    return "soil";
}

// buildex_soil_sensor  buildin_soil;       // Serial.println("Measurement failed!");