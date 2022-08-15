
#include "WiFiThread.h"
#include "utils.h"

WiFiThread::WiFiThread(SysConfig &config) : Thread("WiFiThread", 4096, 1), cfg(config) {
    Start();
}

void WiFiThread::Run() {
    WiFi.begin("se.101_24G", "qqqqqqqq9");
    LOGSS.println("");
    LOGSS.println("WiFi connected");
    LOGSS.println("IP address: ");
    LOGSS.println(WiFi.localIP());
    char *prefix_param = (char *)"TEST=";
    char *filename     = (char *)"config.txt";
    char *param        = (char *)"abcdeddfasdf";
    cfg.WriteConfigParam(filename, prefix_param, param);
    if(WiFi.status() == WL_CONNECTED)
        cfg.wificonnected = true;
    while (true) {
        Delay(Ticks::MsToTicks(10000));
        // LOGSS.print("SSID: ");
        // LOGSS.println(cfg.ssid);
        // LOGSS.print("PASSWORD: ");
        // LOGSS.println(cfg.password);
        //LOGSS.printf("WIFI Stacks Free Bytes Remaining %d\r\n", uxTaskGetStackHighWaterMark(GetHandle()));
    }
    //   while (WiFi.status() != WL_CONNECTED) {
    //     Delay(500);
    //     LOGSS.print(".");
    //   }
    //   LOGSS.println("");
    //   LOGSS.println("WiFi connected");
    //   LOGSS.println("IP address: ");
    //   LOGSS.println(WiFi.localIP());
}
