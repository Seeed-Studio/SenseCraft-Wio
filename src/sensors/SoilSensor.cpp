#include "Arduino.h"
#include "SoilSensor.h"


SoilMoistureSensor::SoilMoistureSensor()
{
}

float SoilMoistureSensor::calculateSD(int datas[], int len)    //计算标准差
{
    float sum = 0.0, mean, standardDeviation = 0.0;
    int i;
    for(i=0; i<len; ++i)
    {
        sum += datas[i];
    }
    mean = sum/len;
    for(i=0; i<len; ++i)
        standardDeviation += pow(datas[i] - mean, 2);
    return sqrt(standardDeviation/len);
}

void SoilMoistureSensor::init()
{
    int sensor_data[20];
    for (int i = 0; i < 20; i++) {
        sensor_data[i] = analogRead(A0);
    }
    if(calculateSD(sensor_data, 20) < 5.0 )
    {
        status = true;
    } else {
        status = false;
    }
}

bool SoilMoistureSensor::read(struct sensor_data *sdata)
{
    sdata->id = SoilMoisture;
    sdata->name = name;

    if (status) {
        soil_value = analogRead(A0);
        sdata->data = &soil_value;
        sdata->size = sizeof(soil_value);
        sdata->status = true;
        return true;
    } else {
        sdata->data   = NULL;
        sdata->status = false;
        init(); // init again, try to find the sensor
        return false;
    }
}

const char *SoilMoistureSensor::get_name()
{
	return "soil-moisture";
}