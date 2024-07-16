#include "MQTT_manager.h"

namespace MQTT_manager {

bool mqttConnected = false;
WiFiClientSecure espClient;
PubSubClient client(espClient);
const char* clientIdPrefix = "Sensor_esp32_client-";
const char* ca_cert = MQTT_CERTIFICATE;
void (*statusCallback)(const char*);

// トピック名の定義は config.h から取得
const char* topicHapbeat = MQTT_TOPIC_HAPBEAT;
const char* topicWebApp = MQTT_TOPIC_WEBAPP;

// ユニークなクライアントIDを生成する関数
String getUniqueClientId() {
  String clientId = clientIdPrefix;
  clientId += String(WiFi.macAddress());
  return clientId;
}

// MQTTブローカーへの接続関数
void reconnect() {
  while (!client.connected()) {
    String clientId = getUniqueClientId();
    Serial.print("Attempting MQTT connection with client ID: ");
    Serial.println(clientId);

    if (statusCallback) {
      statusCallback("Attempting MQTT connection...");
    }

    if (client.connect(clientId.c_str(), MQTT_USERNAME, MQTT_PASSWORD)) {
      Serial.println("connected");
      mqttConnected = true;
      if (statusCallback) {
        statusCallback("connected success");
      }
      // サブスクリプションを再設定（必要なら追加）
      if (client.subscribe(topicHapbeat)) {
        Serial.print("Subscribed to topic: ");
        Serial.println(topicHapbeat);
      } else {
        Serial.println("Subscription to topicHapbeat failed");
      }
      if (client.subscribe(topicWebApp)) {
        Serial.print("Subscribed to topic: ");
        Serial.println(topicWebApp);
      } else {
        Serial.println("Subscription to topicWebApp failed");
      }
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");

      mqttConnected = false;
      if (statusCallback) {
        statusCallback("connect failed");
      }
      delay(5000);
    }
  }
}

void initMQTTclient(void (*statusCb)(const char*)) {
  statusCallback = statusCb;

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
  espClient.setCACert(ca_cert);

  client.setServer(MQTT_SERVER, MQTT_PORT);
  reconnect();
}

void loopMQTTclient() {
  if (!client.connected()) {
    reconnect();
  } else {
    client.loop();
  }
}

// トピックごとのメッセージ送信関数
void sendMessageToHapbeat(const char* message) {
  if (client.publish(topicHapbeat, message)) {
    Serial.print("Message sent to Hapbeat: ");
    Serial.println(message);
  } else {
    Serial.println("Failed to send message to Hapbeat");
  }
}

void sendMessageToWebApp(const char* message) {
  if (client.publish(topicWebApp, message)) {
    Serial.print("Message sent to WebApp: ");
    Serial.println(message);
  } else {
    Serial.println("Failed to send message to WebApp");
  }
}

}  // namespace MQTT_manager
