#ifndef __WIFITHREAD_H__
#define __WIFITHREAD_H__
#include "Seeed_Arduino_ooFreeRTOS.h"
#include "SysConfig.h"
#include <PubSubClient.h>
#include <rpcWiFi.h>
#include <vector>

#define MQTT_BROKER "industrial.api.ubidots.com"

using namespace cpp_freertos;

class WiFiThread : public Thread {
  public:
    WiFiThread(SysConfig &config);
    void WiFiPushData(std::vector<sensor_data *> d);

  protected:
    virtual void Run();

    void reconnect();
    void send_data();

  private:
    SysConfig &cfg;

  private:
    const char              *ssid     = "ssid";
    const char              *password = "password";
    std::vector<sensor_data> wifi_data;
    bool                     wifi_data_ready = true;
    WiFiClient               wifiClient;
    PubSubClient             *client;
};

#endif // __WIFITHREAD_H__