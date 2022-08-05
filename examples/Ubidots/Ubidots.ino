#include "Seeed_Arduino_GroveAI.h"
#include <PubSubClient.h>
#include <rpcWiFi.h>
#include <Wire.h>

#define WIFISSID "STU-EE" // Put your WifiSSID here
#define PASSWORD "stu-ee-2022" // Put your wifi password here
#define TOKEN "BBFF-0Bgo88gCwrbEUmbEVwIQaqHrFW0tGv" // Put your Ubidots' TOKEN
#define VARIABLE_LABEL1 "soil" // Assign the variable label
#define VARIABLE_LABEL2 "num"
#define VARIABLE_LABEL3 "confidence"
#define DEVICE_LABEL "wio-terminal" // Assign the device label
#define MQTT_CLIENT_NAME "o9rp2cwhty" // MQTT client Name

char mqttBroker[] = "industrial.api.ubidots.com";

WiFiClient wifiClient;
PubSubClient client(wifiClient);

GroveAI ai(Wire);
uint8_t state = 0;

int sensorPin = A0;
static int soilValue = 0;
static int num = 0;
static int conf = 0;
static int cont = 0;

// Space to store values to send
char str_soil[6];
char str_num[6];
char str_conf[6];
char payload[700];
char topic[150];

void callback(char* topic, byte* payload, unsigned int length){
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i=0;i<length;i++) {
    Serial.print((char)payload[i]);
  }
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.println("Attempting MQTT connection...");

  // Attempt to connect
  if (client.connect(MQTT_CLIENT_NAME, TOKEN,"")) {
    Serial.println("connected");
  }
  else {
    Serial.print("failed, rc=");
    Serial.print(client.state());
    Serial.println(" try again in 2 seconds");
    // Wait 2 seconds before retrying
    delay(2000);
    }
  }
}

void VisionAI_Init()   //Vision AI 初始化
{
  Serial.println("begin");
  if (ai.begin(ALGO_OBJECT_DETECTION, MODEL_EXT_INDEX_1)) // Object detection and pre-trained model 1
  {
    state = 1;
  }
  else
  {
    Serial.println("Algo begin failed.");
  }
}

void read_VisionAI()  //读VisionAI的值：识别到的人物数量、每个人的置信度
{
  if (state == 1)
  {
    uint32_t tick = millis();
    if (ai.invoke()) // begin invoke
    {
      while (1) // wait for invoking finished
      {
        CMD_STATE_T ret = ai.state(); 
        if (ret == CMD_STATE_IDLE)
        {
          break;
        }
        delay(20);
      }
      uint8_t len = ai.get_result_len(); // receive how many people detect
      if(len)
      {
         Serial.print("Number of people: ");

         num = len;
         Serial.print(num);

         object_detection_t data;       //get data
         for (int i = 0; i < len; i++)
         {
            ai.get_result(i, (uint8_t*)&data, sizeof(object_detection_t)); //get result
            Serial.print("confidence:");
            
            conf = data.confidence;
            Serial.print(conf);
            Serial.println();
          }
      }
     else
     {
       Serial.println("No identification");
       num = 0;
       conf = 0;
     }
   }
    else
    {
      Serial.println("Invoke Failed.");
      delay(1000);
    }
  }
}

void read_soil() //读取土壤传感器的数值
{
  soilValue = analogRead(sensorPin);
  Serial.print("Moisture = ");
  Serial.println(soilValue);
  if (soilValue > 500) cont = 2;
  if (soilValue < 450) cont = 1;
}

void send_data()
{
  dtostrf(soilValue, 4, 0, str_soil);
  dtostrf(num, 4, 0, str_num);
  dtostrf(conf, 4, 0, str_conf);
  
  if (!client.connected()) {
    reconnect();
  }
  // Builds the topic
  sprintf(topic, "%s", ""); // Cleans the topic content
  sprintf(topic, "%s%s", "/v2.0/devices/", DEVICE_LABEL);

  //Builds the payload
  sprintf(payload, "%s", ""); // Cleans the payload
  sprintf(payload, "{\"%s\":", VARIABLE_LABEL1); // Adds the variable label
  sprintf(payload, "%s%s", payload, str_soil); // Adds the value
  sprintf(payload, "%s}", payload); // Closes the dictionary brackets

  client.publish(topic, payload);
  delay(500);

  sprintf(payload, "%s", ""); // Cleans the payload
  sprintf(payload, "{\"%s\":", VARIABLE_LABEL2); // Adds the variable label
  sprintf(payload, "%s%s", payload, str_num); // Adds the value
  sprintf(payload, "%s}", payload); // Closes the dictionary brackets

  client.publish(topic, payload);
  delay(500);

  sprintf(payload, "%s", ""); // Cleans the payload
  sprintf(payload, "{\"%s\":", VARIABLE_LABEL3); // Adds the variable label
  sprintf(payload, "%s%s", payload, str_conf); // Adds the value
  sprintf(payload, "%s}", payload); // Closes the dictionary brackets

  client.publish(topic, payload);
  delay(500);

  client.loop();
}


void setup() {
  Wire.begin();
  Serial.begin(9600);
  
  //while(!Serial);
  
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    WiFi.begin(WIFISSID, PASSWORD);
  }
  
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  WiFi.begin(WIFISSID, PASSWORD);

  delay(1000);
  client.setServer(mqttBroker, 1883);
  client.setCallback(callback);

  VisionAI_Init();
}

void loop() {
  read_soil();
  read_VisionAI();
  send_data();
}
