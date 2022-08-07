/**
 * @file FakeSensor.cpp
 * @author Baozhu Zuo (zuobaozhu@gmail.com)
 * @brief This is a fake sensor drive, which simulates random disconnection of the sensor during use
 * and random acquisition of measured data.
 * @version 0.1
 * @date 2022-08-07
 *
 * @copyright Copyright (c) 2022 Seeed technology inc.
 *
 */
#include "FakeSensor.h"
#include "Arduino.h"

Fake::Fake() : Thread("Fake", 128, 1) {
}
void Fake::Run() {
    uint16_t delay;
    while (true) {
        delay = random(100,2000);
        dsize = random(0,10);
        for (int i = 0; i < dsize; i++) {
            data[i] = random(-10000, 10000);
        }
        status = random(0, 2);
        Delay(Ticks::MsToTicks(delay));
    }
}

FakeSensor::FakeSensor() {
    fake = new Fake();
}

void FakeSensor::init() {
    fake->Start();
}

bool FakeSensor::read(struct sensor_data *data) {
    data->data   = fake->data;
    data->size   = fake->dsize*sizeof(int);
    data->id     = 0;
    data->name   = name;
    data->status = fake->status;
    return true;
}

const char *FakeSensor::get_name() {
    return this->name;
}