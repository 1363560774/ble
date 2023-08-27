#include <Arduino.h>
#include <nvs_flash.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>


#include<WiFi.h>

//wifi部分
const char* ssid =     "test";
const char* password = "test";

uint8_t retx = 0;
uint8_t preretx = 0;
uint8_t txValue = 0;                         //后面需要发送的值
BLEServer *pServer = nullptr;                   //BLEServer指针 pServer
BLECharacteristic *pTxCharacteristic = nullptr; //BLECharacteristic指针 pTxCharacteristic
bool deviceConnected = false;                //本次连接状态
bool oldDeviceConnected = false;             //上次连接状态d
// See the following for generating UUIDs: https://www.uuidgenerator.net/
#define SERVICE_UUID "12a59900-17cc-11ec-9621-0242ac130002" // UART service UUID
#define CHARACTERISTIC_UUID_RX "12a59e0a-17cc-11ec-9621-0242ac130002"
#define CHARACTERISTIC_UUID_TX "12a5a148-17cc-11ec-9621-0242ac130002"

class MyServerCallbacks : public BLEServerCallbacks
{
    void onConnect(BLEServer *pServer)
    {
        deviceConnected = true;
    };

    void onDisconnect(BLEServer *pServer)
    {
        deviceConnected = false;
    }
};

String my_test_str(const char* open_name, const char* key, const char* value);

class MyCallbacks : public BLECharacteristicCallbacks
{
    void onWrite(BLECharacteristic *pCharacteristic)
    {
        std::string rxValue = pCharacteristic->getValue(); //接收信息



        if (rxValue.length() > 0)
        { //向串口输出收到的值

            if (rxValue.length() > 0)
            {
                retx = rxValue[0];


//                nvs_handle_t my_handle;
//                esp_err_t err;
//                char get_char[30] = {0};
//                char set_char = 'A';
//                // Open
//                err = nvs_open("wifi", NVS_READWRITE, &my_handle);
//                if (err != ESP_OK) Serial.println("err");
//
//                // Read
//                size_t required_size = 0;
//                err = nvs_get_str(my_handle, "str_test",nullptr,&required_size);
//                // err = nvs_get_i32(my_handle, "str_test", &restart_counter);
//                // if (err != ESP_OK && err != ESP_ERR_NVS_NOT_FOUND) return err;
//                // err = nvs_get_blob(my_handle, "run_time", NULL, &required_size);
//                if (err != ESP_OK && err != ESP_ERR_NVS_NOT_FOUND) {
//                    Serial.println("err");
//                }
//                Serial.printf("size is %d",required_size);
//                if (required_size > 0) {
//                    err = nvs_get_str(my_handle, "str_test", get_char, &required_size);
//                    Serial.printf("test str is: %s \nsize is %d \n",get_char,required_size);
//                    if (err != ESP_OK) {
//                        Serial.println("err");
//                    }
//                } else {
//                    // Serial.printf("no str data now!!!\n");
//                    // nvs_set_str(my_handle, "str_test", "set_char");
//                    // Serial.printf("set data now!!!");
//                }
//                // Close
//                nvs_close(my_handle);

                String value = my_test_str("wifi", "str_test2", "set_char2");
                Serial.println(value);

//                esp_err_t c = nvs_flash_init();
//                Serial.println(c);
//                nvs_handle_t my_handle_t;
////                esp_err_t err = nvs_open("wifi", NVS_READWRITE, &my_handle_t);
//                if (c != ESP_OK) {
//                    Serial.printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
//                } else {
//                    Serial.printf("Done\n");
//
//                    nvs_close(my_handle_t);
//
//                    //设置wifi账号和用户名
//                    nvs_set_str(my_handle_t, "WiFi_SSID", "SDGH");
//                    nvs_set_str(my_handle_t, "WiFi_PASSWORD", "password");
//                    nvs_commit(my_handle_t);
//
//                    //读取WiFi_SSID
//                    size_t required_size = 2;
//                    char *ssid = nullptr;
//                    nvs_get_str(my_handle_t, "WiFi_SSID", ssid, &required_size);
//                    Serial.println(ssid);
//                    char *wifiName = nullptr;
//                    nvs_get_str(my_handle_t, "password", wifiName, &required_size);
//                    Serial.println(wifiName);
//
//                    // Close
//                    nvs_close(my_handle_t);
//                }

                Serial.println(retx);
            }
            // Serial.print("RX: ");
            // for (int i = 0; i < rxValue.length(); i++)
            //     Serial.print(rxValue[i]);
            // Serial.println("RX: ");
            // Serial.println(rxValue.length());
            // Serial.println(rxValue[0] == 182);
        }
    }
};

//wifi初始化
void wifi_init ()
{
    static char en;
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
        Serial.printf("\r\nConnected to %s\r\n",ssid);
        Serial.printf("\r\nIP address: %s\r\n", WiFi.localIP().toString().c_str());
    }
}

String my_test_str(const char* open_name, const char* key, const char* value) {
    nvs_handle_t my_handle;
    esp_err_t err;
    char get_char[30] = {0};
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
        err = nvs_get_str(my_handle, "str_test", get_char, &required_size);
        Serial.printf("test str is: %s \nsize is %d \n",get_char,required_size);
        if (err != ESP_OK) {
            Serial.println("err");
        }
    } else {
         Serial.printf("no str data now!!!\n");
         nvs_set_str(my_handle, key, value);
         Serial.printf("set data now!!!");
    }
    // Close
    nvs_close(my_handle);
    return get_char;
}

void setup() {
    // write your initialization code here
    Serial.begin(115200);
//    wifi_init();
    // 创建一个 BLE 设备
    BLEDevice::init("BAKUMAN");//在这里面是ble的名称

    // 创建一个 BLE 服务
    pServer = BLEDevice::createServer();
    pServer->setCallbacks(new MyServerCallbacks()); //设置回调
    BLEService *pService = pServer->createService(SERVICE_UUID);

    // 创建一个 BLE 特征
    pTxCharacteristic = pService->createCharacteristic(CHARACTERISTIC_UUID_TX, BLECharacteristic::PROPERTY_NOTIFY);
    pTxCharacteristic->addDescriptor(new BLE2902());
    BLECharacteristic *pRxCharacteristic = pService->createCharacteristic(CHARACTERISTIC_UUID_RX, BLECharacteristic::PROPERTY_WRITE);
    pRxCharacteristic->setCallbacks(new MyCallbacks()); //设置回调

    pService->start();                  // 开始服务
    pServer->getAdvertising()->start(); // 开始广播
    Serial.println(" 等待一个客户端连接，且发送通知... ");
}

void loop() {
    // deviceConnected 已连接
    if (preretx != retx)
    {
        preretx = retx;
        pTxCharacteristic->setValue(&retx, 1); // 设置要发送的值为1
        pTxCharacteristic->notify();              // 广播
        // txValue++;                                // 指针数值自加1
        delay(2000);                              // 如果有太多包要发送，蓝牙会堵塞
    }

    // disconnecting  断开连接
    if (!deviceConnected && oldDeviceConnected)
    {
        delay(500);                  // 留时间给蓝牙缓冲
        pServer->startAdvertising(); // 重新广播
        Serial.println(" 开始广播 ");
        oldDeviceConnected = deviceConnected;
    }

    // connecting  正在连接
    if (deviceConnected && !oldDeviceConnected)
    {
        // do stuff here on connecting
        oldDeviceConnected = deviceConnected;
    }
}