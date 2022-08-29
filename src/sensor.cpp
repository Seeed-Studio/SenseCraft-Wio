#include "sensor.h"

std::vector<log_data>      sensor_base::slog;

void sensor_base::pushlog(const char *msg)
{
    log_data log;
    strcpy(log.data, msg);
    log.time = millis();
    if (slog.size() > LOG_MAX_SIZE) 
    {
        slog.erase(slog.begin()); 
    }
    slog.push_back(log);
}

