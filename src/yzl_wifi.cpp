//
// Created by kai on 2023/8/28.
//
#include <WiFi.h>
#include "yzl_nvs.h"
#include "yzl_utils.h"
#include <PubSubClient.h>
#include <thread>

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

bool mqtt_init();

void callback(char *topic, byte *payload, unsigned int length);

bool check_wifi() {
    return WiFiClass::status() == WL_CONNECTED;
}

// 线程函数
void doWork() {
    if (client.connect("client_id.c_str()", mqtt_username, mqtt_password)) {
        Serial.println("Public emqx mqtt broker connected");
    } else {
        Serial.print("failed with state ");
        Serial.print(client.state());
        delay(2000);
    }
    std::cout << "Worker thread running" << std::endl;
}

//wifi初始化
void wifi_init(const char* in_ssid, const char* in_password)
{
    if (strlen(in_ssid) > 1 && strlen(in_password) > 7) {
        ssid = in_ssid;
        password = in_password;
    }
    static char en;
    if (strlen(ssid) < 1 && strlen(password) < 7) {
        char* nvs_one_wifi = get_nvs_str("wifi", "1", nullptr);
        Serial.printf("nvs_one_wifi=%s\n", nvs_one_wifi);
        if (strstr(nvs_one_wifi, "wifi:ssid&=")) {
            std::vector<string> wifi = str_split(nvs_one_wifi, "|$|");
            std::vector<string> ssid_str = str_split(wifi[0], "&=");
            std::vector<string> password_str = str_split(wifi[1], "&=");
            if (ssid_str[1].length() > 1) {
                ssid = ssid_str[1].data();
                printf("ssid1=%s\n", ssid);
            }
            if (password_str[1].length() > 7) {
                password = password_str[1].data();
                printf("password1=%s\n", password);
            }
        }
    }
    printf("ssid=%s\n", ssid);
    printf("password=%s\n", password);
    WiFi.begin(ssid, password);

    Serial.printf("\r\nConnecting to %s\r\n",ssid);
    while(en != WL_CONNECTED)
    {
        en = WiFiClass::status();
        delay(500);
        Serial.printf(".");
    }
    if (WiFiClass::status() == WL_CONNECTED)
    {
        uint8_t s_len = strlen(ssid);
        uint8_t p_len = strlen(password);
        char result[s_len + p_len + 24];
        sprintf(result, "%s%s%s%s", "wifi:ssid&=", ssid, "|$|password&=", password);
        Serial.printf("result = %s\n",result);
        get_nvs_str("wifi", "1", result);
        Serial.printf("\r\nConnected to %s\r\n",ssid);
        Serial.printf("\r\nIP address: %s\r\n", WiFi.localIP().toString().c_str());
        mqtt_init();
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

bool mqtt_init() {
    client.setServer(mqtt_broker, mqtt_port);
    client.setCallback(callback);
    while (!client.connected()) {
        String client_id = "esp-client-";
        client_id += String(WiFi.macAddress());
        Serial.printf("The client %s connects to the public mqtt broker\n", client_id.c_str());
        // 创建新线程,执行doWork函数
        std::thread t(doWork);
        // 主线程继续执行自己的工作
        std::cout << "Main thread running" << std::endl;
        // 等待线程t完成
        t.join();
    }
    // publish and subscribe
    client.publish(topic, "hello emqx");
    client.subscribe(topic);
    return true;
}

void mqtt_loop() {
    client.loop();
}
