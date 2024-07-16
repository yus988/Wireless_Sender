#ifndef MQTT_MANAGER_H
#define MQTT_MANAGER_H

#include <PubSubClient.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>  // WiFiClientSecureをインクルード

#include "../../config.h"  // プロジェクトのルートからインクルード

namespace MQTT_manager {

extern bool mqttConnected;
extern WiFiClientSecure espClient;
extern PubSubClient client;

void initMQTTclient(void (*statusCb)(const char*));
void loopMQTTclient();

// メッセージ送信関数の宣言
void sendMessageToHapbeat(const char* message);
void sendMessageToWebApp(const char* message);

}  // namespace MQTT_manager

#endif  // MQTT_MANAGER_H
