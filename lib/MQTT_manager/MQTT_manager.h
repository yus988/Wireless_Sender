#ifndef MQTT_MANAGER_H
#define MQTT_MANAGER_H

#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <MQTT.h>  // 256dpi/MQTTライブラリをインクルード

#include "../../include/config.h"  // プロジェクトのルートからインクルード

namespace MQTT_manager {

extern bool mqttConnected;
extern WiFiClientSecure espClient;
extern MQTTClient client;
extern void (*statusCallback)(const char*);

void initMQTTclient(void (*statusCb)(const char*));
void loopMQTTclient();

// メッセージ送信関数の宣言
void sendMessageToHapbeat(const char* message);
void sendMessageToWebApp(const char* message);

}  // namespace MQTT_manager

#endif  // MQTT_MANAGER_H
