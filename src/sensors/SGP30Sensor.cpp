#include "SGP30Sensor.h"
#include "SensorsUtils.h"
#include "sgp30.h"
#include "sensirion_common.h"

SGP30Sensor::SGP30Sensor() {
}

void SGP30Sensor::init() {
    if (I2CScanner(SGP_I2C_ADDRESS, Wire1)) {
        sgp_probe();
        sgp_set_absolute_humidity(13000);
        sgp_iaq_init();
        status = true;
    } else {
        status = false;
    }
}

bool SGP30Sensor::read(struct sensor_data *sdata) {
    u16 tvoc_ppb, co2_eq_ppm;

    sdata->id   = SGP30SENSOR;
    sdata->name = name;

    if (status) {
        if (!I2CScanner(SGP_I2C_ADDRESS, Wire1)) {
            status = false;
            return false;
        }

        sgp_measure_iaq_blocking_read(&tvoc_ppb, &co2_eq_ppm);
        data[0]       = (int)tvoc_ppb;
        data[1]       = (int)co2_eq_ppm;
        sdata->data   = data;
        sdata->size   = sizeof(data);
        sdata->status = true;
        return true;
    } else {
        sdata->data   = NULL;
        sdata->status = false;
        init(); // init again, try to find the sensor
        return false;
    }
}

const char *SGP30Sensor::get_name() {
    return this->name;
}
