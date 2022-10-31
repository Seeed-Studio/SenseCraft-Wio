#include "grove_sgp30_sensor.h"
#include "Arduino.h"
#include "SensorsUtils.h"

grove_sgp30_sensor::grove_sgp30_sensor() { }
void grove_sgp30_sensor::init() {
    if (is_connected) {
        sgp30.begin(softwarei2c);
        sgp30.initAirQuality();
    }
}

bool grove_sgp30_sensor::read(struct sensor_data *sdata) {
    /* check out connection */
    if (!I2C_Detect(grove_i2c_addr[S_SGP30], softwarei2c)) {
        is_connected = false;
        return false;
    }
    else if (!is_connected) { // first connect
        is_connected = true;
        init();
        return false; // waiting for next read
    }

    /* check out measurement */
    if (sgp30.measureAirQuality() != SGP30_SUCCESS) {
        sdata->status = false;
        sgp30.initAirQuality();
    } else {
        sdata->status  = true;
        sgp30_value[0] = sgp30.CO2;
        sgp30_value[1] = sgp30.TVOC;

        sdata->data      = sgp30_value;
        sdata->data_type = SENSOR_DATA_TYPE_INT32;
        sdata->size      = sizeof(sgp30_value);
        sdata->id        = GROVE_SGP30;
        sdata->name      = name;
        sdata->ui_type   = SENSOR_UI_TYPE_NORMAL;
        sdata->data_unit = data_unit;
    }
    return sdata->status;
}

const char *grove_sgp30_sensor::get_name() {
    return "sgp30";
}

// grove_sgp30_sensor  buildin_sgp30;       // Serial.println("Measurement failed!");