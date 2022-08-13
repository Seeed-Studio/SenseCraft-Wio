#include "SHT40Sensor.h"
#include "SensorsUtils.h"

SHT40Sensor::SHT40Sensor() {
}

void SHT40Sensor::init() {
    if (I2CScanner(SHT4X_I2C_ADDRESS, Wire1)) {
        status = true;
    } else {
        status = false;
    }
}

bool SHT40Sensor::read(struct sensor_data *sdata) {
    float temperature, humidity;

    sdata->id   = SHT40SENSOR;
    sdata->name = name;
                
    if (status) {
        if (!I2CScanner(SHT4X_I2C_ADDRESS, Wire1)) {
            status = false;
            return false;
        }

        sht4x.measureHighPrecision(temperature, humidity);
        data[0]       = (int)(temperature * 100);
        data[1]       = (int)(humidity * 100);
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

const char *SHT40Sensor::get_name() {
    return this->name;
}
