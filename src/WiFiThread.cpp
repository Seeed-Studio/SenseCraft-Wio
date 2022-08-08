
#include "WiFiThread.h"

WiFiThread::WiFiThread(SysConfig &config) : Thread("WiFiThread", 4096, 1), cfg(config) {
    Start();
}

void WiFiThread::Run() {
     WiFi.begin("se.101_24G", "qqqqqqqq9");
      Serial.println("");
      Serial.println("WiFi connected");
      Serial.println("IP address: ");
      Serial.println(WiFi.localIP());
    while (true) {
        Delay(Ticks::MsToTicks(100));
    }
    //   while (WiFi.status() != WL_CONNECTED) {
    //     Delay(500);
    //     Serial.print(".");
    //   }
    //   Serial.println("");
    //   Serial.println("WiFi connected");
    //   Serial.println("IP address: ");
    //   Serial.println(WiFi.localIP());
}
