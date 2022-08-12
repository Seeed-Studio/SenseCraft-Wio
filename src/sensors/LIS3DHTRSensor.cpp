#include "LIS3DHTRSensor.h"
#include "SensorsUtils.h"

LIS3DHTRSensor::LIS3DHTRSensor() {
}

void LIS3DHTRSensor::init() {
    if (I2CScanner(LIS3DHTR_DEFAULT_ADDRESS, Wire)) {
        lis.begin(Wire);
        lis.setOutputDataRate(LIS3DHTR_DATARATE_25HZ); // Data output rate
        lis.setFullScaleRange(LIS3DHTR_RANGE_2G);
        status = true;
    } else {
        status = false;
    }
}

bool LIS3DHTRSensor::read(struct sensor_data *sdata) {
    float x_values, y_values, z_values;

    sdata->id   = LIS3DHTRSENSOR;
    sdata->name = name;

    if (status) {
        if (!I2CScanner(LIS3DHTR_DEFAULT_ADDRESS, Wire)) {
            status = false;
            return false;
        }

        lis.getAcceleration(&x_values, &y_values, &z_values);
        data[0]       = (int)(x_values * 100);
        data[1]       = (int)(y_values * 100);
        data[2]       = (int)(z_values * 100);
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

const char *LIS3DHTRSensor::get_name() {
    return this->name;
}
