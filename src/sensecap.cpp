#include "sensecap.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

power_data_t power_val = {
    .fix_1 = MEASURE_FIX,
    .type = MEASURE_POWER_TYPE,
    .fix_2 = MEASURE_FIX,
    .battery_level = FULL_POWER,
    .upload_interval = UPLOAD_INTERVAL,
    .CRC_L = POWER_CRCL,
    .CRC_H = POWER_CRCH,
};

version_data_t version_val = {
    .type = MEASURE_VERSION_TYPE,
    .fix = MEASURE_FIX,
    .hardware_version = HARDWARE_VER,
    .software_version = SOFTWARE_VER,
    .CRC_L = VER_CRCL,
    .CRC_H = VER_CRCH,
}

builtin_sensor_data_t builtin_sensor = {
    .type = MEASURE_BUITIN_DATA_TYPE,
};

other_sensor_data_t other_sensor = {
    .type = MEASURE_OTHER_DATA_TYPE,
};

visionai_sensor_data1_t visionai_data1 = {
    .type = MEASURE_VISIONAI_DATA1_TYPE,
};

visionai_sensor_data2_t visionai_data2 = {
    .type = MEASURE_VISIONAI_DATA2_TYPE,
};

void send_data(void)
{
    // // 发送首包
    // data_start.type = MEASURE_DATA_FIRST_TYPE;
    // if (data_count <= 2)
    //     data_start.type = MEASURE_DATA_ONE_TYPE;
    // data_start.measure_ch = ((value[0].channel + 1) << 4);
    // data_start.sensor_data[0] = BSWAP32(value[0].data);
    // if (data_count == 1)
    //     data_start.sensor_data[1] = 0xFFFFFFFF;
    // else {
    //     data_start.measure_ch = data_start.measure_ch | (value[1].channel + 1);
    //     data_start.sensor_data[1] = BSWAP32(value[1].data);
    // }
    // data_list.len = sizeof(dtu_data_start_t);
    // memcpy(&data_list.data_start, &data_start, sizeof(dtu_data_start_t));
    // osMessageQueuePut(DataQueueHandle, &data_list, 0, 0);

    // //通道数大于4时，还需要发送追加包
    // if (data_count > 4) {
    //     //发送追加包
    //     for (i = 3; i < data_count - 1; i = i + 2) {
    //         data_mid.measure_ch = ((value[i - 1].channel + 1) << 4) | (value[i].channel + 1);
    //         data_mid.sensor_data[0] = BSWAP32(value[i - 1].data);
    //         data_mid.sensor_data[1] = BSWAP32(value[i].data);

    //         data_list.len = sizeof(dtu_data_t);
    //         memcpy(&data_list.data_mid, &data_mid, sizeof(dtu_data_t));
    //         osMessageQueuePut(DataQueueHandle, &data_list, 0, 0);
    //     }
    // }
    // //通道数大于2时，需要分多包
    // if (data_count > 2) {
    //     //发送尾包
    //     data_start.type = MEASURE_DATA_LAST_TYPE;
    //     data_start.measure_ch = ((value[data_count + data_count % 2 - 2].channel + 1) << 4);
    //     data_start.sensor_data[0] = BSWAP32(value[data_count + data_count % 2 - 2].data);
    //     data_start.sensor_data[1] = 0xFFFFFFFF;
    //     if (data_count % 2 == 0) {
    //         data_start.measure_ch =
    //             data_start.measure_ch | (value[data_count + data_count % 2 - 1].channel + 1);
    //         data_start.sensor_data[1] = BSWAP32(value[data_count + data_count % 2 - 1].data);
    //     }
    //     data_list.len = sizeof(dtu_data_start_t);
    //     memcpy(&data_list.data_start, &data_start, sizeof(dtu_data_start_t));
    //     osMessageQueuePut(DataQueueHandle, &data_list, 0, 0);
    // }
    // free(value);
}

