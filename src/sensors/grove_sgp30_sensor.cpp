#include "grove_sgp30_sensor.h"
#include "Arduino.h"
#include "SensorsUtils.h"

Sgp30::Sgp30() : Thread("Sgp30", 128, 1) {
}
void Sgp30::Run() {
    softwarei2c.begin(SGP30_SDAPIN, SGP30_SCLPIN);
    // Initialize sensor
    mySensor.begin(softwarei2c);
    mySensor.initAirQuality();
    while (true) {
        softwarei2c.begin(SGP30_SDAPIN, SGP30_SCLPIN);
        if (mySensor.measureAirQuality() == SGP30_SUCCESS) {
            if (status == false)
                mySensor.initAirQuality();
            status  = true;
            data[0] = mySensor.CO2;
            data[1] = mySensor.TVOC;
        } else {
            status = false;
        }
        Delay(Ticks::MsToTicks(SENSOR_READ_DELAY));
    }
}

grove_sgp30_sensor::grove_sgp30_sensor() {
    // sgp30 = new Sgp30();
}
void grove_sgp30_sensor::init() {
    // sgp30->Start();
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