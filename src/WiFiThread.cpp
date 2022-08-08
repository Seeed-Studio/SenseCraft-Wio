
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
    char *prefix_param = (char *)"TEST=";
    char *filename     = (char *)"config.txt";
    char *param        = (char *)"abcdeddfasdf";
    cfg.WriteConfigParam(filename, prefix_param, param);
    while (true) {
        Delay(Ticks::MsToTicks(1000));
        Serial.print("SSID: ");
        Serial.println(cfg.ssid);
        Serial.print("PASSWORD: ");
        Serial.println(cfg.password);
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
