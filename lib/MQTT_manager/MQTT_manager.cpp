#include "MQTT_manager.h"

namespace MQTT_manager {

bool mqttConnected = false;
WiFiClientSecure espClient;
PubSubClient client(espClient);
const char* clientIdPrefix = "Sensor_esp32_client-";

// PubSubClientのコールバック関数を設定する変数
// void (*mqttCallback)(char*, byte*, unsigned int);
void (*statusCallback)(const char*);

// void callback(char* topic, byte* payload, unsigned int length) {
//   if (mqttCallback) {
//     mqttCallback(topic, payload, length);
//   }
// }

const char* ca_cert = MQTT_CERTIFICATE;

// // コールバック関数
// void callback(char* topic, byte* payload, unsigned int length) {
//   Serial.print("Message arrived in topic: ");
//   Serial.println(topic);

//   Serial.print("Message: ");
//   for (int i = 0; i < length; i++) {
//     Serial.print((char)payload[i]);
//   }
//   Serial.println();
// }

// ユニークなクライアントIDを生成する関数
String getUniqueClientId() {
  String clientId = clientIdPrefix;
  clientId += String(WiFi.macAddress());
  return clientId;
}

// MQTTブローカーへの接続関数
void reconnect() {
  // 再接続の試行をループ
  while (!client.connected()) {
    String clientId = getUniqueClientId();
    Serial.print("Attempting MQTT connection with client ID: ");
    Serial.println(clientId);

    if (statusCallback) {
      statusCallback("Attempting MQTT connection...");
    }

    // ユーザー名とパスワードを使用して接続
    if (client.connect(clientId.c_str(), MQTT_USERNAME, MQTT_PASSWORD)) {
      Serial.println("connected");
      mqttConnected = true;
      if (statusCallback) {
        statusCallback("connected success");
      }
      // サブスクリプションを再設定
      if (client.subscribe(MQTT_TOPIC)) {
        Serial.print("Subscribed to topic: ");
        Serial.println(MQTT_TOPIC);
      } else {
        Serial.println("Subscription failed");
      }
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");

      // 接続失敗の理由を表示
      switch (client.state()) {
        case -4:
          Serial.println(
              "MQTT_CONNECTION_TIMEOUT - the server didn't respond within the "
              "keepalive time");
          break;
        case -3:
          Serial.println(
              "MQTT_CONNECTION_LOST - the network connection was broken");
          break;
        case -2:
          Serial.println(
              "MQTT_CONNECT_FAILED - the network connection failed");
          break;
        case -1:
          Serial.println(
              "MQTT_DISCONNECTED - the client is disconnected cleanly");
          break;
        case 0:
          Serial.println("MQTT_CONNECTED - the client is connected");
          break;
        case 1:
          Serial.println(
              "MQTT_CONNECT_BAD_PROTOCOL - the server doesn't support the "
              "requested version of MQTT");
          break;
        case 2:
          Serial.println(
              "MQTT_CONNECT_BAD_CLIENT_ID - the server rejected the client "
              "identifier");
          break;
        case 3:
          Serial.println(
              "MQTT_CONNECT_UNAVAILABLE - the server was unable to accept the "
              "connection");
          break;
        case 4:
          Serial.println(
              "MQTT_CONNECT_BAD_CREDENTIALS - the username/password were "
              "rejected");
          break;
        case 5:
          Serial.println(
              "MQTT_CONNECT_UNAUTHORIZED - the client was not authorized to "
              "connect");
          break;
        default:
          Serial.println("Unknown error");
          break;
      }

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
  // client.setCallback(callback);
  reconnect();
}

void loopMQTTclient() {
  // 接続状態の確認と再接続
  if (!client.connected()) {
    reconnect();
  } else {
    // メッセージの処理と接続の維持
    client.loop();
  }
}

}  // namespace MQTT_manager
