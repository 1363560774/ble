//
// Created by kai on 2023/8/28.
//
#include <WiFi.h>
#include "yzl_nvs.h"
#include "yzl_utils.h"
#include <PubSubClient.h>

//wifi部分
const char* ssid = "your_wifi_sidd";
const char* password = "your_wifi_password";

int wifi_nvs_index = 1;

// MQTT Broker
const char *mqtt_broker = "your_mqtt_host";
const char *topic = "esp/mq";
const char *mqtt_username = "username";
const char *mqtt_password = "password";
const int mqtt_port = 1883;

WiFiClient espClient;
PubSubClient client(espClient);

//wifi初始化
void wifi_init ()
{
    static char en;
    if (ssid == nullptr && password == nullptr) {
        char* nvs_one_wifi = get_nvs_str("wifi", "1", nullptr);
        std::vector<string> wifi = str_split(nvs_one_wifi, plit);
        if (!wifi.empty()) {
            ssid = wifi[0].data();
            password = wifi[1].data();
        }
    }
    WiFi.begin(ssid, password);

    Serial.printf("\r\nConnecting to %s\r\n",ssid);
    while(en != WL_CONNECTED)
    {
        en = WiFiClass::status();
        delay(500);
        Serial.printf(".");
    }
    if(WiFiClass::status() == WL_CONNECTED)
    {
        uint8_t s_len = strlen(ssid);
        uint8_t p_len = strlen(ssid);
        char result[s_len + p_len];
        get_nvs_str("wifi", "1", result);
        Serial.printf("\r\nConnected to %s\r\n",ssid);
        Serial.printf("\r\nIP address: %s\r\n", WiFi.localIP().toString().c_str());
    }
}

void callback(char *topic, byte *payload, unsigned int length) {
    Serial.print("Message arrived in topic: ");
    Serial.println(topic);
    Serial.print("Message:");
    for (int i = 0; i < length; i++) {
        Serial.print((char) payload[i]);
    }
    Serial.println();
    Serial.println("-----------------------");
}

void mqtt_init() {
    client.setServer(mqtt_broker, mqtt_port);
    client.setCallback(callback);
    while (!client.connected()) {
        String client_id = "esp-client-";
        client_id += String(WiFi.macAddress());
        Serial.printf("The client %s connects to the public mqtt broker\n", client_id.c_str());
        if (client.connect(client_id.c_str(), mqtt_username, mqtt_password)) {
            Serial.println("Public emqx mqtt broker connected");
        } else {
            Serial.print("failed with state ");
            Serial.print(client.state());
            delay(2000);
        }
    }
    // publish and subscribe
    client.publish(topic, "hello emqx");
    client.subscribe(topic);
}

void mqtt_loop() {
    client.loop();
}
