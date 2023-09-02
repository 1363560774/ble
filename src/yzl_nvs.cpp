//
// Created by kai on 2023/8/28.
//

#include <Arduino.h>
#include "yzl_nvs.h"
#include <nvs_flash.h>

char get_char[30] = {0};

char* get_nvs_str(const char* open_name, const char* key, const char* value) {
    nvs_handle_t my_handle;
    esp_err_t err;
    // Open
    err = nvs_open(open_name, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) Serial.println("err");

    // Read
    size_t required_size = 0;
    err = nvs_get_str(my_handle, key,nullptr,&required_size);
    // err = nvs_get_i32(my_handle, "str_test", &restart_counter);
    // if (err != ESP_OK && err != ESP_ERR_NVS_NOT_FOUND) return err;
    // err = nvs_get_blob(my_handle, "run_time", NULL, &required_size);
    if (err != ESP_OK && err != ESP_ERR_NVS_NOT_FOUND) {
        Serial.println("err");
    }
    Serial.printf("size is %d",required_size);
    if (required_size > 0) {
        err = nvs_get_str(my_handle, key, get_char, &required_size);
        Serial.printf("test str is: %s \nsize is %d \n",get_char,required_size);
        if (err != ESP_OK) {
            Serial.println("err");
        }
    } else {
        if (value != nullptr) {
            nvs_set_str(my_handle, key, value);
            Serial.printf("set data success %s \n", value);
        }
    }
    // Close
    nvs_close(my_handle);
    return get_char;
}

void set_nvs_str(const char* open_name, const char* key, const char* value) {
    nvs_handle_t my_handle;
    esp_err_t err;
    // Open
    err = nvs_open(open_name, NVS_READWRITE, &my_handle);
    if (err == ESP_OK) {
        nvs_set_str(my_handle, key, value);
    }
}
