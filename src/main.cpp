#include <Arduino.h>
#include "yzl_ble.h"
#include "yzl_wifi.h"

void setup() {
    // write your initialization code here
    Serial.begin(115200);
    ble_init();
    wifi_init();
    mqtt_init();
}

void loop() {
    ble_loop();
    mqtt_loop();
}