
#include "WiFiThread.h"

WiFiThread::WiFiThread(SysConfig &config) : Thread("WiFiThread", 1024, 1), cfg(config) {
    Start();
}

void WiFiThread::Run() {
    //WiFi.begin(ssid, password);
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
