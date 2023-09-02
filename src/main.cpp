#include <Arduino.h>
#include <BLECharacteristic.h>
#include "yzl_ble.h"
#include "yzl_wifi.h"
#include "yzl_utils.h"
#include <vector>

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

void IRAM_ATTR buttonInterrupt() {
    GPIO.status = 1 << ble_button_pin;
}

void setup() {
    // write your initialization code here
    Serial.begin(115200);
    pinMode(ble_button_pin, INPUT_PULLUP);
    //中断写法
//    attachInterrupt(ble_button_pin, buttonInterrupt, RISING);
}

class MyCallbacks : public BLECharacteristicCallbacks
{
    void onWrite(BLECharacteristic *pCharacteristic) override
    {
        std::string rxValue = pCharacteristic->getValue(); //接收信息
        if (rxValue.length() > 0)
        { //向串口输出收到的值
            Serial.printf("rxValue=%s\n", rxValue.c_str());
            string ssid_ = "ssid";
            string password_ = "password";
            if (strstr(rxValue.c_str(), "wifi:ssid&=")) {
                std::vector<string> wifi = str_split(rxValue, "|$|");
                std::vector<string> ssid_str = str_split(wifi[0], "&=");
                std::vector<string> password_str = str_split(wifi[1], "&=");
                if (ssid_str[1].length() > 1) {
                    in_ssid = ssid_str[1].data();
                }
                if (password_str[1].length() > 7) {
                    in_password = password_str[1].data();
                }
            }
            printf("in_ssid=%s\n", in_ssid);
            printf("in_password=%s\n", in_password);
        }
    }
};

void loop() {
    now = millis();
    button_state = digitalRead(ble_button_pin);
    if (button_state == LOW && now - last_button_press > open_ble_time) {
        last_button_press = now;
        if (!open_ble) {
            Serial.println("蓝牙打开");
            ble_init(new MyCallbacks);
            printf("in_ssid=%s && in_password=%s\n", in_ssid, in_password);
            wifi_init(in_ssid, in_password);
            mqtt_init();
        }
        open_ble = true;
    } else if (button_state == HIGH) {
        last_button_press = now;
    }
    if (open_ble) {
        ble_loop();
    }
    mqtt_loop();
}