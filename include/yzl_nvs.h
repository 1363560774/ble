//
// Created by kai on 2023/8/28.
//

#ifndef BLE_YZL_NVS_H
#define BLE_YZL_NVS_H
#include <Arduino.h>


class yzl_nvs {

};
char* get_nvs_str(const char* open_name, const char* key, const char* value);
void set_nvs_str(const char* open_name, const char* key, const char* value);
#endif //BLE_YZL_NVS_H
