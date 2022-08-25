#include "buildex_sht4x_sensor.h"
#include "SoftwareI2C.h"

buildex_sht4x_sensor::buildex_sht4x_sensor() {
}
void buildex_sht4x_sensor::init() {
    softwarei2c.begin(SHT4X_SDAPIN, SHT4X_SCLPIN);
    sht4x.setPort(softwarei2c);
    sht4x.setChipType(SHT4X_CHIPTYPE_A);
    sht4x.setMode(SHT4X_CMD_MEAS_HI_PREC);
    sht4x.checkSerial();
}

bool buildex_sht4x_sensor::read(struct sensor_data *sdata) {
    softwarei2c.begin(SHT4X_SDAPIN, SHT4X_SCLPIN);
    if (sht4x.measure() != SHT4X_STATUS_OK) {
        return false;
    }
    if (sht4x.TcrcOK) {
        sht4x_value[0] = (uint8_t)sht4x.TtoDegC();
    } else {
        sht4x_value[0] = 0xFF;
    }
    if (sht4x.RHcrcOK) {
        sht4x_value[1] = (uint8_t)sht4x.RHtoPercent();
    } else {
        sht4x_value[1] = 0xFF;
    }
    sdata->data   = &sht4x_value;
    sdata->size   = sizeof(sht4x_value);
    sdata->id     = BUILDEX_SHT4X;
    sdata->name   = name;
    sdata->status = true;
    return true;
}

const char *buildex_sht4x_sensor::get_name() {
    return "sht4x";
}

// buildex_sht4x_sensor  buildin_sht4x;