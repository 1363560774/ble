#include <Arduino.h>
#include <BLECharacteristic.h>
#include "yzl_ble.h"
#include "yzl_wifi.h"
#include "yzl_utils.h"
#include <vector>
#include <iostream>

using namespace std;

//蓝牙按键引脚
const int ble_button_pin = 0;
//按键状态
int button_state = 0;
//开机时间
unsigned long now;
//上次按键时间
unsigned long last_button_press = 0;
//长按3秒开启蓝牙
int open_ble_time = 3000;
//蓝牙打开,默认不打开
bool open_ble = false;
const char* in_ssid = "";
const char* in_password = "";
bool mq_connected = false;

// 定义PWM 频道和引脚
const int pwmChannel = 0;
const int pwmPin = 5;

// 50Hz频率
const int freq = 50;

void IRAM_ATTR buttonInterrupt() {
    GPIO.status = 1 << ble_button_pin;
}

void setup() {
    // write your initialization code here
    Serial.begin(115200);
    pinMode(ble_button_pin, INPUT_PULLUP);
    // 配置PWM 通道
    ledcSetup(pwmChannel, freq, 16);

    // 将通道0绑定到GPIO5
    ledcAttachPin(pwmPin, pwmChannel);

    //中断写法
//    attachInterrupt(ble_button_pin, buttonInterrupt, RISING);
}

class MyCallbacks : public BLECharacteristicCallbacks
{
    void onWrite(BLECharacteristic *pCharacteristic) override
    {
        std::string rxValue = pCharacteristic->getValue(); //接收信息
        if (rxValue.length() < 0)
            return;
        //向串口输出收到的值
        Serial.printf("rxValue=%s\n", rxValue.c_str());
        string duo = rxValue.substr(0, 6);
        if (duo == "duo_ji") {
            int x;
            int j;
            try {
                j = true;
                string c = rxValue.substr(6, rxValue.length());
                x = stoi(c);
            } catch (invalid_argument& e) {
                j = false;
                // 处理非数字字符串情况
            } catch (out_of_range& e) {
                // 处理超出范围情况
                x = 8400;
            }
            if (j) {
                ledcWrite(pwmChannel, x * 84);
                printf("Rotate to x=%d, duty=%d degree \n", x, x * 84);
                delay(20);
            }
            return;
        }
        string pre = rxValue.substr(0, 11);
        if (pre != "wifi:ssid&=")
            return;
        if (check_wifi())
            return;
        std::vector<string> wifi = str_split(rxValue, "|$|");
        std::vector<string> ssid_str = str_split(wifi[0], "&=");
        std::vector<string> password_str = str_split(wifi[1], "&=");
        if (ssid_str[1].length() < 1 && password_str[1].length() < 7)
            return;
        in_ssid = ssid_str[1].data();
        in_password = password_str[1].data();
        printf("in_ssid=%s && in_password=%s\n", in_ssid, in_password);
        wifi_init(in_ssid, in_password);
        if (check_wifi()) {
            mq_connected = mqtt_init();
        }
    }
};

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

void loop() {
    now = millis();
    button_state = digitalRead(ble_button_pin);
    if (button_state != LOW && now - last_button_press > open_ble_time) {
        last_button_press = now;
        if (!open_ble) {
            Serial.println("蓝牙打开");
            ble_init(new MyCallbacks);
        }
        open_ble = true;
    } else if (button_state == HIGH) {
        last_button_press = now;
    }
    if (open_ble) {
        ble_loop();
    }
    if (mq_connected) {
        mqtt_loop();
    }
}