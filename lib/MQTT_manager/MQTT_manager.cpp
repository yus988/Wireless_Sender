#include "MQTT_manager.h"

namespace MQTT_manager {

bool mqttConnected = false;
WiFiClientSecure espClient;
MQTTClient client(256);
const char* clientIdPrefix = "Sensor_esp32_client-";
const char* ca_cert = MQTT_CERTIFICATE;
void (*statusCallback)(const char*);

// トピック名の定義は config.h から取得
const char* topicHapbeat = MQTT_TOPIC_HAPBEAT;
const char* topicWebApp = MQTT_TOPIC_WEBAPP;
const int QoS_Val = 1;  // 0=once, 1=least once, 2=exact once

// ユニークなクライアントIDを生成する関数
String getUniqueClientId() {
  String clientId = clientIdPrefix;
  clientId += String(WiFi.macAddress());
  return clientId;
}

// MQTTメッセージ到着時のコールバック関数
void messageReceived(String& topic, String& payload) {
  Serial.println("Message arrived: ");
  Serial.println("  topic: " + topic);
  Serial.println("  payload: " + payload);
}

// MQTTブローカーへの接続関数
void connect() {
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
      if (client.subscribe(topicHapbeat, 1)) {
        Serial.print("Subscribed to topic: ");
        Serial.println(topicHapbeat);
        statusCallback("Successfully connected to Hapbeat");
      } else {
        mqttConnected = false;
        Serial.println("Subscription to topicHapbeat failed");
      }
      if (client.subscribe(topicWebApp, 1)) {
        Serial.print("Subscribed to topic: ");
        Serial.println(topicWebApp);
      } else {
        mqttConnected = false;
        Serial.println("Subscription to topicWebApp failed");
      }
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.lastError());
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

  // 証明書の設定
  espClient.setCACert(ca_cert);
  espClient.setTimeout(20000);  // タイムアウト設定

  client.begin(MQTT_SERVER, MQTT_PORT, espClient);
  // client.onMessage(messageReceived);
  connect();

  // 誤って retain 送ってしまった場合にリセット
    // client.publish(topicHapbeat, "", true,
    //               QoS_Val);  // 空のペイロードとRetainedフラグをtrueに設定
    // client.publish(topicWebApp, "", true,
    //               QoS_Val);  // 空のペイロードとRetainedフラグをtrueに設定
}

void loopMQTTclient() {
  if (!client.connected()) {
    connect();
  }
  client.loop();
}

// トピックごとのメッセージ送信関数
void sendMessageToHapbeat(const char* message) {
  if (client.publish(topicHapbeat, message, true, QoS_Val)) {
    Serial.print("Message sent to Hapbeat: ");
    Serial.println(message);
  } else {
    Serial.println("Failed to send message to Hapbeat");
  }
}

void sendMessageToWebApp(const char* message) {
  if (client.publish(topicWebApp, message, false, QoS_Val)) {
    Serial.print("Message sent to WebApp: ");
    Serial.println(message);
  } else {
    Serial.println("Failed to send message to WebApp");
  }
}

}  // namespace MQTT_manager
