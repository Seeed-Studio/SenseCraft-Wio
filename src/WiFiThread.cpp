
#include "WiFiThread.h"
#include "PubSubClient.h"
#include "utils.h"
#include <functional>

const char *ROOT_CA_BALTIMORE = "-----BEGIN CERTIFICATE-----\n"
                                "MIIDdzCCAl+gAwIBAgIEAgAAuTANBgkqhkiG9w0BAQUFADBaMQswCQYDVQQGEwJJ\n"
                                "RTESMBAGA1UEChMJQmFsdGltb3JlMRMwEQYDVQQLEwpDeWJlclRydXN0MSIwIAYD\n"
                                "VQQDExlCYWx0aW1vcmUgQ3liZXJUcnVzdCBSb290MB4XDTAwMDUxMjE4NDYwMFoX\n"
                                "DTI1MDUxMjIzNTkwMFowWjELMAkGA1UEBhMCSUUxEjAQBgNVBAoTCUJhbHRpbW9y\n"
                                "ZTETMBEGA1UECxMKQ3liZXJUcnVzdDEiMCAGA1UEAxMZQmFsdGltb3JlIEN5YmVy\n"
                                "VHJ1c3QgUm9vdDCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBAKMEuyKr\n"
                                "mD1X6CZymrV51Cni4eiVgLGw41uOKymaZN+hXe2wCQVt2yguzmKiYv60iNoS6zjr\n"
                                "IZ3AQSsBUnuId9Mcj8e6uYi1agnnc+gRQKfRzMpijS3ljwumUNKoUMMo6vWrJYeK\n"
                                "mpYcqWe4PwzV9/lSEy/CG9VwcPCPwBLKBsua4dnKM3p31vjsufFoREJIE9LAwqSu\n"
                                "XmD+tqYF/LTdB1kC1FkYmGP1pWPgkAx9XbIGevOF6uvUA65ehD5f/xXtabz5OTZy\n"
                                "dc93Uk3zyZAsuT3lySNTPx8kmCFcB5kpvcY67Oduhjprl3RjM71oGDHweI12v/ye\n"
                                "jl0qhqdNkNwnGjkCAwEAAaNFMEMwHQYDVR0OBBYEFOWdWTCCR1jMrPoIVDaGezq1\n"
                                "BE3wMBIGA1UdEwEB/wQIMAYBAf8CAQMwDgYDVR0PAQH/BAQDAgEGMA0GCSqGSIb3\n"
                                "DQEBBQUAA4IBAQCFDF2O5G9RaEIFoN27TyclhAO992T9Ldcw46QQF+vaKSm2eT92\n"
                                "9hkTI7gQCvlYpNRhcL0EYWoSihfVCr3FvDB81ukMJY2GQE/szKN+OMY3EU/t3Wgx\n"
                                "jkzSswF07r51XgdIGn9w/xZchMB5hbgF/X++ZRGjD8ACtPhSNzkE1akxehi/oCr0\n"
                                "Epn3o0WC4zxe9Z2etciefC7IpJ5OCBRLbf1wbWsaY71k5h+3zvDyny67G7fyUIhz\n"
                                "ksLi4xaNmjICq44Y3ekQEe5+NauQrz4wlHrQMz2nZQ/1/I6eYs9HRCwBXbsdtTLS\n"
                                "R9I4LtD+gdwyah617jzV/OeBHRnDJELqYzmp\n"
                                "-----END CERTIFICATE-----";
void callback(char *topic, byte *payload, unsigned int length) {
    LOGSS.print("WIFI - Message arrived [");
    LOGSS.print(topic);
    LOGSS.print("] ");
    for (int i = 0; i < length; i++) {
        LOGSS.print((char)payload[i]);
    }
    LOGSS.println("");
}

WiFiThread::WiFiThread(SysConfig &config) : Thread("WiFiThread", 128 * 6, 1), cfg(config) {
    Start();
}

void WiFiThread::Run() {
    client = new PubSubClient(wifiClient);
    ntp    = new NTP(wifi_udp);
    while (true) {
        if (cfg.wifi_on) {
            if (!cfg.wificonnected) {
            // while (WiFi.status() != WL_CONNECTED) {
                WiFi.begin(cfg.ssid.begin(), cfg.password.begin());
                LOGSS.println("WIFI - Connecting to WiFi...");
                Delay(Ticks::MsToTicks(1000));
                if (WiFi.status() == WL_CONNECTED) { 
                    LOGSS.println("WIFI - Configuring MQTT...");
                    if (cfg.cloud == CLOUD_AZURE) {
                        ntp->begin();
#if defined(USE_DPS)
                        if (RegisterDeviceToDPS(IOT_CONFIG_GLOBAL_DEVICE_ENDPOINT, cfg.id_scope.c_str(),
                                                cfg.registration_id.c_str(), cfg.symmetric_key.c_str(),
                                                ntp->epoch() + TOKEN_LIFESPAN) != 0) {
                            WiFi.disconnect();
                        }
#else
                        HubHost  = IOT_CONFIG_IOTHUB;
                        DeviceId = IOT_CONFIG_DEVICE_ID;
#endif // USE_DPS
                    }
                    else { // Ubidots
                        client->setServer(MQTT_BROKER, 1883);
                        client->setCallback(callback);
                    }
                    cfg.wificonnected = true;
                    cfg.wifi_rssi     = WiFi.RSSI();
                }
            }
            send_data();
            for(int i=0; i<60; i++){
                if (cfg.wifi_on) { // wifi always on
                    Delay(Ticks::MsToTicks(1000));
                } else { // wifi turn off
                    cfg.wificonnected = false;
                    break;
                }
            }
        } else { // wifi off
            client->disconnect();
            WiFi.disconnect();
            cfg.wificonnected = false;
            Delay(Ticks::MsToTicks(1000));
        }
    }
}

// Store the received sensor data into a queue of length 30.
void WiFiThread::WiFiPushData(std::vector<sensor_data *> d) {
    // A loop to deep copy param of d vector into new wifi_data queue
    // by Iterative method
    if (wifi_data_ready) {
        wifi_data.clear();
        wifi_data.shrink_to_fit();
        for (auto data : d)
            wifi_data.push_back(*data);
    }
}

void WiFiThread::MqttSubscribeCallbackDPS(char *topic, byte *payload, unsigned int length) {
    LOGSS.printf("Subscribe: \r\n %s \r\n Len:%d \r\n", topic, length);
    if (DpsClient.RegisterSubscribeWork(topic, std::vector<uint8_t>(payload, payload + length)) !=
        0) {
        LOGSS.println("Failed to parse topic and/or payload");
        return;
    }

    if (!DpsClient.IsRegisterOperationCompleted()) {
        const int waitSeconds = DpsClient.GetWaitBeforeQueryStatusSeconds();
        LOGSS.printf("Querying after %u  seconds...\r\n", waitSeconds);

        DpsPublishTimeOfQueryStatus = millis() + waitSeconds * 1000;
    }
}
void WiFiThread::MqttSubscribeCallbackHub(char *topic, byte *payload, unsigned int length) {
    az_span                          topic_span = az_span_create((uint8_t *)topic, strlen(topic));
    az_iot_hub_client_method_request command_request;

    if (az_result_succeeded(az_iot_hub_client_methods_parse_received_topic(&HubClient, topic_span,
                                                                           &command_request))) {
        // DisplayPrintf("Command arrived!");
        // Determine if the command is supported and take appropriate actions
        HandleCommandMessage(az_span_create(payload, length), &command_request);
    }

    // LOGSS.print(DLM);
}

////////////////////////////////////////////////////////////////////////////////
// Azure IoT DPS
int WiFiThread::RegisterDeviceToDPS(const std::string &endpoint, const std::string &idScope,
                                    const std::string &registrationId,
                                    const std::string &symmetricKey,
                                    const uint64_t    &expirationEpochTime) {
    std::string endpointAndPort{endpoint};
    endpointAndPort += ":";
    endpointAndPort += std::to_string(8883);

    if (DpsClient.Init(endpointAndPort, idScope, registrationId) != 0)
        return -1;

    LOGSS.printf(
        "DPS:\r\n Endpoint: %s\r\n IdScope: %s\r\n RegistrationId: %s\r\n SymmetricKey: %s\r\n",
        endpoint.c_str(), idScope.c_str(), registrationId.c_str(), symmetricKey.c_str());

    const std::string mqttClientId = DpsClient.GetMqttClientId();
    LOGSS.printf(" MQTT client id = %s\r\n", mqttClientId.c_str());
    const std::string mqttUsername = DpsClient.GetMqttUsername();
    LOGSS.printf(" MQTT username = %s\r\n", mqttUsername.c_str());
    const std::vector<uint8_t> signature = DpsClient.GetSignature(expirationEpochTime);
    const std::string encryptedSignature = GenerateEncryptedSignature(symmetricKey, signature);
    const std::string mqttPassword =
        DpsClient.GetMqttPassword(encryptedSignature, expirationEpochTime);
    // LOGSS.printf(" MQTT password = %s\r\n", mqttPassword.c_str());
    const std::string registerPublishTopic   = DpsClient.GetRegisterPublishTopic();
    const std::string registerSubscribeTopic = DpsClient.GetRegisterSubscribeTopic();

    wifiClient.setCACert(ROOT_CA_BALTIMORE);
    client->setBufferSize(MQTT_PACKET_SIZE);
    client->setServer(endpoint.c_str(), 8883);
    std::function<void(char *, byte *, unsigned int)> callback =
        std::bind(&WiFiThread::MqttSubscribeCallbackDPS, this, std::placeholders::_1,
                  std::placeholders::_2, std::placeholders::_3);
    client->setCallback(callback);
    if (!client->connect(mqttClientId.c_str(), mqttUsername.c_str(), mqttPassword.c_str()))
        return -2;

    client->subscribe(registerSubscribeTopic.c_str());
    client->publish(registerPublishTopic.c_str(),
                    "{payload:{\"modelId\":\"" IOT_CONFIG_MODEL_ID "\"}}");

    while (!DpsClient.IsRegisterOperationCompleted()) {
        client->loop();
        if (DpsPublishTimeOfQueryStatus > 0 && millis() >= DpsPublishTimeOfQueryStatus) {
            client->publish(DpsClient.GetQueryStatusPublishTopic().c_str(), "");
            LOGSS.println("Client sent operation query message");
            DpsPublishTimeOfQueryStatus = 0;
        }
        Delay(Ticks::MsToTicks(50));
    }

    if (!DpsClient.IsAssigned())
        return -3;

    client->disconnect();

    HubHost  = DpsClient.GetHubHost();
    DeviceId = DpsClient.GetDeviceId();

    LOGSS.printf("Device provisioned:\r\n Hub Host: %s\r\n Device Id: %s\r\n", HubHost.c_str(),
                 DeviceId.c_str());
    return 0;
}

////////////////////////////////////////////////////////////////////////////////
// Azure IoT Hub
int WiFiThread::ConnectToHub(az_iot_hub_client *iot_hub_client, const std::string &symmetricKey,
                             const uint64_t &expirationEpochTime) {
    static std::string deviceIdCache;
    deviceIdCache = DeviceId;

    const az_span hostSpan{az_span_create((uint8_t *)&HubHost[0], HubHost.size())};
    LOGSS.printf("Hub:\r\n Host: %s\r\n", HubHost.c_str());
    
    const az_span deviceIdSpan{az_span_create((uint8_t *)&deviceIdCache[0], deviceIdCache.size())};
    LOGSS.printf(" Device id = %s\r\n", deviceIdCache.c_str());

    az_iot_hub_client_options options = az_iot_hub_client_options_default();
    options.model_id                  = AZ_SPAN_LITERAL_FROM_STR(IOT_CONFIG_MODEL_ID);
    if (az_result_failed(az_iot_hub_client_init(iot_hub_client, hostSpan, deviceIdSpan, &options)))
        return -1;

    char   mqttClientId[128];
    size_t client_id_length;
    if (az_result_failed(az_iot_hub_client_get_client_id(iot_hub_client, mqttClientId,
                                                         sizeof(mqttClientId), &client_id_length)))
        return -4;

    LOGSS.printf(" MQTT client id = %s\r\n", mqttClientId);
    // LogMemoryUsage(__FUNCTION__);
    char mqttUsername[256];
    if (az_result_failed(az_iot_hub_client_get_user_name(iot_hub_client, mqttUsername,
                                                         sizeof(mqttUsername), NULL)))
        return -5;
    LOGSS.printf(" MQTT username = %s\r\n", mqttUsername);
    // LogMemoryUsage(__FUNCTION__);
    char    mqttPassword[300];
    uint8_t signatureBuf[256];
    az_span signatureSpan = az_span_create(signatureBuf, sizeof(signatureBuf));
    az_span signatureValidSpan;
    if (az_result_failed(az_iot_hub_client_sas_get_signature(iot_hub_client, expirationEpochTime,
                                                             signatureSpan, &signatureValidSpan)))
        return -2;
    const std::vector<uint8_t> signature(az_span_ptr(signatureValidSpan),
                                         az_span_ptr(signatureValidSpan) +
                                             az_span_size(signatureValidSpan));
    const std::string encryptedSignature = GenerateEncryptedSignature(symmetricKey, signature);
    az_span           encryptedSignatureSpan =
        az_span_create((uint8_t *)&encryptedSignature[0], encryptedSignature.size());
    if (az_result_failed(az_iot_hub_client_sas_get_password(
            iot_hub_client, expirationEpochTime, encryptedSignatureSpan, AZ_SPAN_EMPTY,
            mqttPassword, sizeof(mqttPassword), NULL)))
        return -3; 
    // LOGSS.printf(" MQTT password = %s\r\n", mqttPassword);

    wifiClient.setCACert(ROOT_CA_BALTIMORE);
    client->setBufferSize(MQTT_PACKET_SIZE);
    client->setServer(HubHost.c_str(), 8883);
    std::function<void(char *, byte *, unsigned int)> callback =
        std::bind(&WiFiThread::MqttSubscribeCallbackHub, this, std::placeholders::_1,
                  std::placeholders::_2, std::placeholders::_3);
    client->setCallback(callback);

    if (!client->connect(mqttClientId, mqttUsername, mqttPassword))
        return -6;

    client->subscribe(AZ_IOT_HUB_CLIENT_METHODS_SUBSCRIBE_TOPIC);
    client->subscribe(AZ_IOT_HUB_CLIENT_C2D_SUBSCRIBE_TOPIC);
    LOGSS.println("connect to hub end");
    // LogMemoryUsage(__FUNCTION__);

    return 0;
}

void WiFiThread::HandleCommandMessage(az_span                           payload,
                                      az_iot_hub_client_method_request *command_request) {
    int       command_res_code = 200;
    az_result rc               = AZ_OK;

    if (az_span_is_content_equal(AZ_SPAN_LITERAL_FROM_STR("ringBuzzer"), command_request->name)) {
        // Parse the command payload (it contains a 'duration' field)
        LOGSS.println("Processing command \'ringBuzzer\'");
        char buffer[32];
        az_span_to_str(buffer, 32, payload);
        LOGSS.printf("Raw command payload: %s\r\n", buffer);

        az_json_reader json_reader;
        uint32_t       duration;
        if (az_json_reader_init(&json_reader, payload, NULL) == AZ_OK) {
            if (az_json_reader_next_token(&json_reader) == AZ_OK) {
                if (az_result_failed(rc =
                                         az_json_token_get_uint32(&json_reader.token, &duration))) {
                    LOGSS.printf("Couldn't parse JSON token res=%d\r\n", rc);
                } else {
                    LOGSS.printf("Duration: %dms\r\n", duration);
                }
            }

            // Invoke command
            analogWrite(WIO_BUZZER, 128);
            delay(duration);
            analogWrite(WIO_BUZZER, 0);

            int rc;
            if (az_result_failed(rc = SendCommandResponse(command_request, command_res_code,
                                                          AZ_SPAN_LITERAL_FROM_STR("{}")))) {
                LOGSS.printf("Unable to send %d response, status 0x%08x\r\n", command_res_code, rc);
            }
        }
    } else {
        // Unsupported command
        LOGSS.printf("Unsupported command received: %.*s.\r\n", az_span_size(command_request->name),
                     az_span_ptr(command_request->name));

        int rc;
        if (az_result_failed(
                rc = SendCommandResponse(command_request, 404, AZ_SPAN_LITERAL_FROM_STR("{}")))) {
            LOGSS.printf("Unable to send %d response, status 0x%08x\n", 404, rc);
        }
    }
}

int WiFiThread::SendCommandResponse(az_iot_hub_client_method_request *request, uint16_t status,
                                    az_span response) {
    az_result rc = AZ_OK;
    // Get the response topic to publish the command response
    char commands_response_topic[128];
    if (az_result_failed(rc = az_iot_hub_client_methods_response_get_publish_topic(
                             &HubClient, request->request_id, status, commands_response_topic,
                             sizeof(commands_response_topic), NULL))) {
        LOGSS.println("Unable to get method response publish topic");
        return rc;
    }

    LOGSS.printf("Status: %u\tPayload: '", status);
    char *payload_char = (char *)az_span_ptr(response);
    if (payload_char != NULL) {
        for (int32_t i = 0; i < az_span_size(response); i++) {
            LOGSS.printf("%c", *(payload_char + i));
        }
    }
    LOGSS.println();

    // Send the commands response
    if (client->publish(commands_response_topic, az_span_ptr(response), az_span_size(response),
                        false)) {
        LOGSS.println("Sent response");
    }

    return rc;
}

void WiFiThread::reconnect() {
    LOGSS.println("WIFI - Attempting MQTT connection...");
    // Attempt to connect
    if (cfg.cloud == CLOUD_AZURE) {
        const uint64_t now = ntp->epoch();
        if (ConnectToHub(&HubClient, cfg.symmetric_key.c_str(), now + TOKEN_LIFESPAN) == 0) {
            LOGSS.println("WIFI - Attempting MQTT connected");
        } else {
            LOGSS.printf(" Hub host = %s\r\n", HubHost.c_str());
            LOGSS.printf(" registration id = %s\r\n", cfg.registration_id.c_str());
            LOGSS.printf("WIFI - Attempting MQTT failed, rc=%d\r\n", client->state());
            LOGSS.println(" try again in 2 seconds");
            Delay(Ticks::MsToTicks(2000)); // Wait 2 seconds before retrying
        }
        reconnectTime = now + TOKEN_LIFESPAN * 0.85;
    } else { //Ubidots
        if (client->connect(cfg.mqtt_client_name.begin(), cfg.token.begin(), "")) {
            LOGSS.println("WIFI - Attempting MQTT connected");
        } else {
            LOGSS.println(cfg.mqtt_client_name.begin());
            LOGSS.println(cfg.token.begin());
            LOGSS.printf("WIFI - Attempting MQTT failed, rc=%d\r\n", client->state());
            LOGSS.println(" try again in 2 seconds");
            Delay(Ticks::MsToTicks(2000)); // Wait 2 seconds before retrying
        }
    }
}

// Sending data to Cloud
void WiFiThread::send_data() {
    wifi_data_ready = false; // disable data push
    while (!client->connected()) {
        reconnect();
    }
    if (cfg.cloud == CLOUD_AZURE)
    {
        LOGSS.println("WIFI - Publishing to azure");
        publish_azure();
    } else { //Ubidots
        LOGSS.println("WIFI - Publishing to ubidots");
        publish_ubidots();
    }
    client->loop();
    wifi_data_ready = true; // enable data push
}

az_result WiFiThread::publish_azure(){
    char telemetry_topic[128];
    if (az_result_failed(az_iot_hub_client_telemetry_get_publish_topic(
            &HubClient, NULL, telemetry_topic, sizeof(telemetry_topic), NULL))) {
        LOGSS.println("Failed az_iot_hub_client_telemetry_get_publish_topic");
        return AZ_ERROR_NOT_SUPPORTED;
    }
    if ((uint64_t)ntp->epoch() >= reconnectTime) {
        LOGSS.printf("ntp->epoch() = %ld \r\n", (uint32_t)ntp->epoch());
        LOGSS.printf("reconnectTime = %ld\r\n", (uint32_t)reconnectTime);
        LOGSS.println("Disconnect");
        client->disconnect();
        return AZ_ERROR_HTTP_ADAPTER;
    }
    az_json_writer json_builder;
    char           telemetry_payload[200];
    // Builds the topic
    AZ_RETURN_IF_FAILED(
        az_json_writer_init(&json_builder, AZ_SPAN_FROM_BUFFER(telemetry_payload), NULL));
    AZ_RETURN_IF_FAILED(az_json_writer_append_begin_object(&json_builder));
    for (auto data : wifi_data) {
        // Builds the payload
        if (data.size / 4 <= 1) {
            if (data.data_type == SENSOR_DATA_TYPE_FLOAT) {
                AZ_RETURN_IF_FAILED(az_json_writer_append_property_name(
                    &json_builder, AZ_SPAN_LITERAL_FROM_CHAR(data.name)));
                AZ_RETURN_IF_FAILED(az_json_writer_append_double(
                    &json_builder, ((int32_t *)data.data)[0] / 100.0f, 2));
            } else {
                AZ_RETURN_IF_FAILED(az_json_writer_append_property_name(
                    &json_builder, AZ_SPAN_LITERAL_FROM_CHAR(data.name)));
                AZ_RETURN_IF_FAILED(
                    az_json_writer_append_int32(&json_builder, ((int32_t *)data.data)[0]));
            }
        } else {
            char payload[16];
            for (int i = 0; i < data.size / 4; i++) {
                sprintf(payload, "%s%d", data.name, i);
                if (data.data_type == SENSOR_DATA_TYPE_FLOAT) {
                    AZ_RETURN_IF_FAILED(az_json_writer_append_property_name(
                        &json_builder, AZ_SPAN_LITERAL_FROM_CHAR(payload)));
                    AZ_RETURN_IF_FAILED(az_json_writer_append_double(
                        &json_builder, ((int32_t *)data.data)[i] / 100.0f, 2));
                } else {
                    AZ_RETURN_IF_FAILED(az_json_writer_append_property_name(
                        &json_builder, AZ_SPAN_LITERAL_FROM_CHAR(payload)));
                    AZ_RETURN_IF_FAILED(
                        az_json_writer_append_int32(&json_builder, ((int32_t *)data.data)[i]));
                }
            }
        }
    }
    const az_span out_payload{az_json_writer_get_bytes_used_in_destination(&json_builder)};
    static int    sendCount = 0;
    if (!client->publish(telemetry_topic, az_span_ptr(out_payload), az_span_size(out_payload),
                         false)) {
        LOGSS.printf("ERROR: Send telemetry %d\r\n", sendCount);
    } else {
        ++sendCount;
        LOGSS.printf("Sent telemetry %d\r\n", sendCount);
    }
}

void WiFiThread::publish_ubidots(){
    char payload[700];
    char topic[150];
    // Builds the topic
    sprintf(topic, "%s", ""); // Cleans the topic content
    sprintf(topic, "%s%s", "/v2.0/devices/", cfg.device_label.begin());
    for (auto data : wifi_data) {
        // Builds the payload
        sprintf(payload, "%s", "");
        if (data.size / 4 <= 1) {
            sprintf(payload, "%s", "");              // Cleans the payload
            sprintf(payload, "{\"%s\":", data.name); // Adds the variable label
            if (data.data_type == SENSOR_DATA_TYPE_FLOAT)
                sprintf(payload, "%s %f", payload, ((int32_t *)data.data)[0] / 100.0f);
            else
                sprintf(payload, "%s %d", payload, ((int32_t *)data.data)[0]); // Adds the value
            sprintf(payload, "%s}", payload); // Closes the dictionary brackets
            client->publish(topic, payload);
            LOGSS.println(payload);
        } else {
            for (int i = 0; i < data.size / 4; i++) {
                sprintf(payload, "%s", "");
                sprintf(payload, "{\"%s%d\":", data.name, i + 1); // Adds the variable label
                if (data.data_type == SENSOR_DATA_TYPE_FLOAT)
                    sprintf(payload, "%s %f", payload, ((int32_t *)data.data)[i] / 100.0f);
                else
                    sprintf(payload, "%s %d", payload, ((int32_t *)data.data)[i]); // Adds the value
                sprintf(payload, "%s}", payload); // Closes the dictionary brackets
                client->publish(topic, payload);
                LOGSS.println(payload);
                Delay(Ticks::MsToTicks(1000));
            }
        }
        Delay(Ticks::MsToTicks(1000));
    }
}