//
// Created by kai on 2023/8/28.
//

#ifndef BLE_YZL_WIFI_H
#define BLE_YZL_WIFI_H


class yzl_wifi {

};
bool check_wifi();
void wifi_init(const char* in_ssid, const char* in_password);
void callback(char *topic, byte *payload, unsigned int length);
bool mqtt_init();
void mqtt_loop();
#endif //BLE_YZL_WIFI_H
