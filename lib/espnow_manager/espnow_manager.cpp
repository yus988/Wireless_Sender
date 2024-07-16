#include <WiFi.h>
#include <esp_now.h>

#ifndef NO_DISPLAY
  #include <M5Unified.h>
#endif

namespace espnowManager {

esp_now_peer_info_t slave;
// data = [category, wearer_id, device_pos, data_id, sub_id, L_Vol, R_Vol,
// isLoop ] isLoop, 0=oneshot, 1=loopStart, 2=stop

#define ELEMENTS_NUM 8 /**< カンマ区切りデータの項目数 */
#define DATA_SIZE sizeof(uint8_t[ELEMENTS_NUM])
int sendTimes = 0;
unsigned int beginIndex;  // 要素の開始位置
static String elements[ELEMENTS_NUM];

const uint8_t data_empty[] = {0, 0, 0, 0, 0, 000, 000, 0};
const uint8_t data_BtnA[] = {0, 0, 99, 5, 0, 150, 150, 1};
const uint8_t data_BtnB[] = {0, 0, 99, 2, 0, 150, 150, 0};
const uint8_t data_BtnC[] = {0, 0, 99, 2, 0, 150, 150, 2};

// const uint8_t data_BtnA[] = {0, 0, 99, 5, 0, 50, 50, 1};
// const uint8_t data_BtnB[] = {0, 0, 5, 2, 0, 150, 150, 0};
// const uint8_t data_BtnC[] = {0, 0, 0, 2, 0, 150, 150, 0};

// uint8_t data[] = data_A;
// esp_err_t result = esp_now_send(slave.peer_addr, data, sizeof(data));

void SendEspNOW(const char* cmd) {
  const uint8_t* data;
  if (cmd == "BtnA") {
    data = data_BtnA;
  } else if (cmd == "BtnB") {
    data = data_BtnB;
  } else if (cmd == "BtnC") {
    data = data_BtnC;
  } else if (cmd == "btnB") {
    data = data_BtnA;
  } else if (cmd == "btnY") {
    data = data_BtnC;
  } else if (cmd == "btnX") {
    data = data_BtnB;
  } else {
    data = data_empty;
  }
  esp_now_send(slave.peer_addr, data, DATA_SIZE);
}

// 送信コールバック
void OnDataSent(const uint8_t* mac_addr, esp_now_send_status_t status) {
  char macStr[18];
  snprintf(macStr, sizeof(macStr), "%02X:%02X:%02X:%02X:%02X:%02X", mac_addr[0],
           mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.print("Last Packet Sent to: ");
  Serial.println(macStr);
  Serial.print("Last Packet Send Status: ");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success"
                                                : "Delivery Fail");
}

void initEspNow(void) {
  // ESP-NOW初期化
  WiFi.mode(WIFI_STA);
  WiFi.setSleep(false);
  WiFi.disconnect();
  if (esp_now_init() == ESP_OK) {
    Serial.println("ESPNow Init Success");
  } else {
    Serial.println("ESPNow Init Failed");
    ESP.restart();
  }
  // マルチキャスト用Slave登録
  memset(&slave, 0, sizeof(slave));
  for (int i = 0; i < 6; ++i) {
    slave.peer_addr[i] = (uint8_t)0xff;
  }
  slave.channel = 0;
  slave.encrypt = false;
  esp_err_t addStatus = esp_now_add_peer(&slave);
  if (addStatus == ESP_OK) {
    // Pair success
    Serial.println("Pair success");
  } else {
    Serial.println("Pair failed");
  }
  // ESP-NOWコールバック登録
  esp_now_register_send_cb(OnDataSent);
}

/*
// [category, pos, id, subid, L_Vol, R_Vol]
category = 大枠のチャンネル（ディスプレイに表示されるチャンネル）
pos = 装着位置（各Hapbeatごとに設定 or ボタン操作で切り替え
id = 音声の種類（銃撃、ダメージなど）
subid = 同じ音声種類内での微小差分（連続するイベントでちょっとずつ変えたい場合）
L_Vol = 左側の振動強度
R_Vol = 右側の振動強度
ex "0,0,0,0,100,100"
*/

void sendSerialViaESPNOW(void) {
  if (Serial.available() > 0) {
    // 終了文字まで取得
    String str = Serial.readStringUntil('\n');
    beginIndex = 0;
    for (uint8_t i = 0; i < ELEMENTS_NUM; i++) {
      if (i != (ELEMENTS_NUM - 1)) {
        uint8_t endIndex;
        endIndex = str.indexOf(',', beginIndex);
        // カンマが見つかった場合
        if (endIndex != -1) {
          elements[i] = str.substring(beginIndex, endIndex);
          beginIndex = endIndex + 1;
        } else {
          break;
        }
      } else {
        elements[i] = str.substring(beginIndex);
      }
    }
    uint8_t data[ELEMENTS_NUM];
    for (uint8_t i = 0; i < ELEMENTS_NUM; i++) {
      data[i] = elements[i].toInt();
    }
    esp_now_send(slave.peer_addr, data, sizeof(data));

#if defined(ENABLE_DISPLAY)
    M5.Display.clear(TFT_BLACK);
    M5.Display.setCursor(0, 100);
    M5.Display.setTextColor(TFT_WHITE);
    M5.Display.setTextSize(2);
    M5.Display.printf("Send Times: %d\n", sendTimes);
    M5.Display.printf("Input: %s\n", str.c_str());
    sendTimes += 1;
    M5.update();
#endif
  }
}

}