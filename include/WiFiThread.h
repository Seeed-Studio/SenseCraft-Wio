#ifndef __WIFITHREAD_H__
#define __WIFITHREAD_H__
#include "Seeed_Arduino_ooFreeRTOS.h"
#include <PubSubClient.h>
#include <rpcWiFi.h>
#include "SysConfig.h"

using namespace cpp_freertos;



class WiFiThread : public Thread {
  public:
    WiFiThread(SysConfig &config);

  protected:
    virtual void Run();

    private:
    SysConfig &cfg;

  private:
    const char *ssid     = "ssid";
    const char *password = "password";
};

#endif // __WIFITHREAD_H__