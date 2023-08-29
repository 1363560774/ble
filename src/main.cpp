#include <Arduino.h>
#include "yzl_ble.h"
#include "yzl_wifi.h"

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

void IRAM_ATTR buttonInterrupt() {
    GPIO.status = 1 << ble_button_pin;
}

void setup() {
    // write your initialization code here
    Serial.begin(115200);
    pinMode(ble_button_pin, INPUT_PULLUP);
    //中断写法
//    attachInterrupt(ble_button_pin, buttonInterrupt, RISING);
    wifi_init();
    mqtt_init();
}

void loop() {
    now = millis();
    button_state = digitalRead(ble_button_pin);
    if (button_state == LOW && now - last_button_press > open_ble_time) {
        last_button_press = now;
        if (!open_ble) {
            Serial.println("蓝牙打开");
            ble_init();
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