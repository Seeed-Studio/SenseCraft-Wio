
#include "WiFiThread.h"
#include "utils.h"

WiFiThread::WiFiThread(SysConfig &config) : Thread("WiFiThread", 2048, 1), cfg(config) {
    Start();
}

void WiFiThread::Run() {
    // LOGSS.println(cfg.ssid.begin());
    // LOGSS.println(cfg.password.begin());
    while (true) {
        if (cfg.wifi_on) {
            while (WiFi.status() != WL_CONNECTED) {
                WiFi.begin(cfg.ssid.begin(), cfg.password.begin());
                // LOGSS.println("Connecting to WiFi...");
                Delay(Ticks::MsToTicks(1000));
            }
            // LOGSS.println(" wifi connected");
            cfg.wificonnected = true;
            // LOGSS.print("SSID: ");
            // LOGSS.println(cfg.ssid);
            // LOGSS.print("PASSWORD: ");
            // LOGSS.println(cfg.password);
            // LOGSS.printf("WIFI Stacks Free Bytes Remaining %d\r\n",
            // uxTaskGetStackHighWaterMark(GetHandle()));
            Delay(Ticks::MsToTicks(1000));
        } else
            Delay(Ticks::MsToTicks(1000));
    }
}