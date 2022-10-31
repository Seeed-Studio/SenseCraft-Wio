#include "grove_vl53l0x_sensor.h"
#include "Arduino.h"
#include "SensorsUtils.h"
#include "SoftwareI2C.h"

grove_vl53l0x_sensor::grove_vl53l0x_sensor() { }
void grove_vl53l0x_sensor::init() {
    if (is_connected) {
        VL53L0X_Error ret = VL53L0X_ERROR_NONE;
        ret = VL53L0X.VL53L0X_common_init();
        ret = VL53L0X.VL53L0X_high_accuracy_ranging_init();
        // VL53L0X.VL53L0X_high_speed_ranging_init();
        // VL53L0X.VL53L0X_long_distance_ranging_init();
        // VL53L0X.VL53L0X_single_ranging_init();
        if (ret != VL53L0X_ERROR_NONE)
            LOGSS.printf("init failed, error code: %d\r\n", ret);
    }
}

bool grove_vl53l0x_sensor::read(struct sensor_data *sdata) {
    /* check out connection */
    if (!I2C_Detect(grove_i2c_addr[S_VL53LX0], softwarei2c)) {
        is_connected = false;
        return false;
    }
    else if (!is_connected) { // first connect
        is_connected = true;
        init();
        return false; // waiting for next read
    }

    /* check out measurement */
    VL53L0X_RangingMeasurementData_t range_data;
    VL53L0X_Error ret = VL53L0X.PerformSingleRangingMeasurement(&range_data);
    if (ret != VL53L0X_ERROR_NONE) {
        LOGSS.printf("mesurement failed, error code : %d\r\n", ret);
        memset(&range_data, 0, sizeof(VL53L0X_RangingMeasurementData_t));
        sdata->status = false;
    } else {
        vl53l0x_value = range_data.RangeMilliMeter;
        if (vl53l0x_value > 2000) {
            vl53l0x_value = 2000;
            // LOGSS.println("VL53L0X : out of range");
        }
        sdata->data      = &vl53l0x_value;
        sdata->data_type = SENSOR_DATA_TYPE_INT32;
        sdata->size      = sizeof(vl53l0x_value);
        sdata->id        = GROVE_VL53L0X;
        sdata->name      = name;
        sdata->ui_type   = SENSOR_UI_TYPE_NORMAL;
        sdata->data_unit = data_unit;
        sdata->status    = true;
    }
    return sdata->status;
}

const char *grove_vl53l0x_sensor::get_name() {
    return "vl53l0x";
}