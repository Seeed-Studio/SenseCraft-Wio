#include "grove_sht4x_sensor.h"
#include "Arduino.h"
#include "SensorsUtils.h"
#include "SoftwareI2C.h"

grove_sht4x_sensor::grove_sht4x_sensor() { }
void grove_sht4x_sensor::init() {
    if (is_connected) {
        sht4x.setPort(softwarei2c);
        sht4x.setChipType(SHT4X_CHIPTYPE_A);
        sht4x.setMode(SHT4X_CMD_MEAS_HI_PREC);
        sht4x.checkSerial();
    }
}

bool grove_sht4x_sensor::read(struct sensor_data *sdata) {
    /* check out connection */
    if (!I2C_Detect(grove_i2c_addr[S_SHT4X], softwarei2c)) {
        is_connected = false;
        return false;
    }
    else if (!is_connected) { // first connect
        is_connected = true;
        init();
        return false; // waiting for next read
    }

    /* check out measurement */
    if (sht4x.measure() != SHT4X_STATUS_OK) {
        sdata->status = false;
    } else {
        sdata->status = true;
        sht4x_value[0] = (sht4x.TcrcOK) ? (int32_t)(sht4x.TtoDegC() * 100) : 0xFF;
        sht4x_value[1] = (sht4x.RHcrcOK) ? (int32_t)(sht4x.RHtoPercent() * 100) : 0xFF;
        sdata->data      = sht4x_value;
        sdata->data_type = SENSOR_DATA_TYPE_FLOAT;
        sdata->size      = sizeof(sht4x_value);
        sdata->id        = GROVE_SHT4X;
        sdata->name      = name;
        sdata->ui_type   = SENSOR_UI_TYPE_NORMAL;
        sdata->data_unit = data_unit;
        return sdata->status;
    }
    return sdata->status;
}

const char *grove_sht4x_sensor::get_name() {
    return "sht4x";
}
