#ifndef __WIFITHREAD_H__
#define __WIFITHREAD_H__
#include "AzureDpsClient.h"
#include "Seeed_Arduino_ooFreeRTOS.h"
#include "Signature.h"
#include "SysConfig.h"
#include <NTP.h>
#include <PubSubClient.h>
#include <WiFiUdp.h>
#include <azure/core/az_json.h>
#include <azure/core/az_result.h>
#include <azure/core/az_span.h>
#include <azure/iot/az_iot_hub_client.h>
#include <rpcWiFiClientSecure.h>
#include <vector>

#define USE_DPS
#define MQTT_PACKET_SIZE 1024
#define TOKEN_LIFESPAN 3600
#ifndef USE_DPS
#define IOT_CONFIG_IOTHUB "global.azure-devices-provisioning.net"
#define IOT_CONFIG_DEVICE_ID "qian-test"
#endif
#define IOT_CONFIG_GLOBAL_DEVICE_ENDPOINT "global.azure-devices-provisioning.net"
#define IOT_CONFIG_MODEL_ID "dtmi:seeedkk:wioterminal:wioterminal_aziot_example;5"

using namespace cpp_freertos;

class WiFiThread : public Thread {
  public:
    WiFiThread(SysConfig &config);
    void WiFiPushData(std::vector<sensor_data *> d);

  protected:
    virtual void Run();

    int  ConnectToHub(az_iot_hub_client *iot_hub_client, const std::string &symmetricKey,
                      const uint64_t &expirationEpochTime);
    int  RegisterDeviceToDPS(const std::string &endpoint, const std::string &idScope,
                             const std::string &registrationId, const std::string &symmetricKey,
                             const uint64_t &expirationEpochTime);
    void MqttSubscribeCallbackDPS(char *topic, byte *payload, unsigned int length);
    void MqttSubscribeCallbackHub(char *topic, byte *payload, unsigned int length);
    void HandleCommandMessage(az_span payload, az_iot_hub_client_method_request *command_request);
    int  SendCommandResponse(az_iot_hub_client_method_request *request, uint16_t status,
                             az_span response);
    void reconnect();
    az_result send_data();

  private:
    SysConfig &cfg;

  private:
    const char              *ssid     = "ssid";
    const char              *password = "password";
    std::vector<sensor_data> wifi_data;
    bool                     wifi_data_ready = true;
    WiFiClientSecure         wifiClient;
    PubSubClient            *client;
    WiFiUDP                  wifi_udp;
    NTP                     *ntp;
    std::string              HubHost;
    std::string              DeviceId;
    AzureDpsClient           DpsClient;
    unsigned long            DpsPublishTimeOfQueryStatus = 0;
    az_iot_hub_client        HubClient;
    uint64_t                 reconnectTime;
};

#endif // __WIFITHREAD_H__