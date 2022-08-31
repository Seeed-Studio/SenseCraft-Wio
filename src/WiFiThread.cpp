
#include "WiFiThread.h"
#include "PubSubClient.h"
#include "utils.h"

char         mqttBroker[] = "industrial.api.ubidots.com";
WiFiClient   wifiClient;
PubSubClient client(wifiClient);

char payload[700];
char topic[150];

WiFiThread::WiFiThread(SysConfig &config) : Thread("WiFiThread", 2048, 1), cfg(config) {
    Start();
}

void callback(char *topic, byte *payload, unsigned int length) {
    LOGSS.print("WIFI - Message arrived [");
    LOGSS.print(topic);
    LOGSS.print("] ");
    for (int i = 0; i < length; i++) {
        LOGSS.print((char)payload[i]);
    }
    LOGSS.println("");
}
void WiFiThread::reconnect() {
    // Loop until we're reconnected
    while (!client.connected()) {
        LOGSS.println("WIFI - Attempting MQTT connection...");

        // Attempt to connect
        if (client.connect(cfg.mqtt_client_name.begin(), cfg.token.begin(), "")) {
            LOGSS.println("WIFI - Attempting MQTT connected");
        } else {
            LOGSS.println(cfg.mqtt_client_name.begin());
            LOGSS.println(cfg.token.begin());
            LOGSS.print("WIFI - Attempting MQTT failed, rc=");
            LOGSS.print(client.state());
            LOGSS.println(" try again in 2 seconds");
            // Wait 2 seconds before retrying
            delay(2000);
        }
    }
}

// Sending data to Ubidots
void WiFiThread::send_data() {
    if (!client.connected()) {
        reconnect();
    }
    // Builds the topic
    sprintf(topic, "%s", ""); // Cleans the topic content
    sprintf(topic, "%s%s", "/v2.0/devices/", cfg.device_label.begin());
    for (auto data : wifi_data) {
        // Builds the payload
        sprintf(payload, "%s", ""); 
        if(data.size / 4 <= 1)
        {                 
        sprintf(payload, "%s", "");                   // Cleans the payload
        sprintf(payload, "{\"%s\":", data.name);                       // Adds the variable label
        sprintf(payload, "%s %d", payload, ((int32_t *)data.data)[0]); // Adds the value
        sprintf(payload, "%s}", payload); // Closes the dictionary brackets
        client.publish(topic, payload);
        // LOGSS.println(payload);
        }
        else
        {
            for(int i = 0; i < data.size / 4; i++)
            {
                sprintf(payload, "%s", ""); 
                sprintf(payload, "{\"%s%d\":", data.name,i+1);                       // Adds the variable label
                sprintf(payload, "%s %d", payload, ((int32_t *)data.data)[i]); // Adds the value
                sprintf(payload, "%s}", payload); // Closes the dictionary brackets
                client.publish(topic, payload);
                // LOGSS.println(payload);
                delay(500);
            }
        }
        
        delay(500);
    }
    client.loop();
}

void WiFiThread::Run() {
    // LOGSS.println(cfg.ssid.begin());
    // LOGSS.println(cfg.password.begin());
    while (true) {
        if (cfg.wifi_on) {
            while (WiFi.status() != WL_CONNECTED) {
                WiFi.begin(cfg.ssid.begin(), cfg.password.begin());
                LOGSS.println("WIFI - Connecting to WiFi...");
                Delay(Ticks::MsToTicks(1000));
                if (WiFi.status() == WL_CONNECTED) {
                    client.setServer(mqttBroker, 1883);
                    client.setCallback(callback);
                }
            }
            LOGSS.println("WIFI -  wifi connected");
            cfg.wificonnected = true;
            send_data(); // Sending data to Ubidots
            wifi_data.clear();
            wifi_data.shrink_to_fit();
            wifi_data_ready = true;
            Delay(Ticks::MsToTicks(5000));
        } else
            Delay(Ticks::MsToTicks(1000));
    }
}

// Store the received sensor data into a queue of length 30.
void WiFiThread::WiFiPushData(std::vector<sensor_data *> d) {
    // A loop to deep copy param of d vector into new wifi_data queue
    // by Iterative method
    if (wifi_data_ready) {
        for (auto data : d)
            wifi_data.push_back(*data);

        wifi_data_ready = false;
    }
}