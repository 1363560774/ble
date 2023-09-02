//
// Created by kai on 2023/8/28.
//

#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLE2902.h>
#include "yzl_nvs.h"
#include <chrono>
#include <sstream>
#include <iomanip>

uint8_t retx = 0;
uint8_t preretx = 0;
uint8_t txValue = 0;                         //后面需要发送的值
BLEServer *pServer = nullptr;                   //BLEServer指针 pServer
BLECharacteristic *pTxCharacteristic = nullptr; //BLECharacteristic指针 pTxCharacteristic
bool deviceConnected = false;                //本次连接状态
bool oldDeviceConnected = false;             //上次连接状态d
// See the following for generating UUIDs: https://www.uuidgenerator.net/
#define SERVICE_UUID "cb5e77db-0e92-4287-81cf-ddcbd332088d" // UART service UUID
#define CHARACTERISTIC_UUID_RX "77ea1bc5-c98d-4748-a1b8-f554ebf91953"
#define CHARACTERISTIC_UUID_TX "9f0d4f79-0905-425b-986d-d835ad92cd6c"

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

class MyCallbacks : public BLECharacteristicCallbacks
{
    void onWrite(BLECharacteristic *pCharacteristic)
    {
        std::string rxValue = pCharacteristic->getValue(); //接收信息
        if (rxValue.length() > 0)
        { //向串口输出收到的值
            Serial.println(retx);
        }
    }
};

std::string get_current_time() {

    auto now = std::chrono::system_clock::now();

    // 获取当前时间点的时间戳
    auto timestamp = std::chrono::system_clock::to_time_t(now);

    // 把时间戳转换为本地时间
    std::tm* local_time = std::localtime(&timestamp);

    std::stringstream ss;

    // 设置输出格式
    ss << std::put_time(local_time, "%Y-%m-%d %H:%M");

    return ss.str();
}

void ble_init() {
    std::string deviceName = "YZL-" + get_current_time();
    // 创建一个 BLE 设备
    BLEDevice::init(deviceName);//在这里面是ble的名称

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

void ble_loop() {
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
