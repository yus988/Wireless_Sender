#include <Arduino.h>
#include <ArduinoJson.h>

#include "./config.h"

#ifdef ENABLE_COLOR_SENSOR
  #include <ColorSensor.h>
#endif

#ifndef NO_DISPLAY
  #include <M5UImanager.h>
#endif

#ifdef ESPNOW
  #include <espnow_manager.h>
#elif MQTT
  #include <MQTT_manager.h>
#endif

const char* cmd_stat;
const char* cmd_btn;
const char* cmd_conn;

String lastColor = "None";

void statusCallback(const char* status) { Serial.printf(status); }

TaskHandle_t thp[2];
void TaskColorSensor(void* args) {
  static uint8_t r, g, b;
  while (1) {
    ColorSensor::loopColorSensor();
    ColorSensor::getColorValues(r, g, b);

    String color = ColorSensor::determineColor(r, g, b);
    Serial.printf("color is: ");
    Serial.println(color);
    // "None"から他の色に変わった場合に、Hapbeatにメッセージを送信
    // 色が変わった場合にメッセージを送信
    if (color != lastColor) {
      if (lastColor == "None" || color != "None") {
        // 送信するメッセージの生成
        int id, lVol;
        if (color == "Red") {
          id = RED_ID;
          lVol = RED_VOL;
        } else if (color == "Green") {
          id = GREEN_ID;
          lVol = GREEN_VOL;
        } else if (color == "Yellow") {
          id = YELLOW_ID;
          lVol = YELLOW_VOL;
        }
        int rVol = lVol;
        char message[100];
        snprintf(message, sizeof(message), "%d,%d,%d,%d,%d,%d,%d,%d", CATEGORY,
                 WEARER_ID, DEVICE_POS, id, SUB_ID, lVol, rVol, PLAY_CMD);

        // 全てのHapbeatから応答があるまで while
        // でメッセージを送り続ける処理を入れる
        MQTT_manager::sendMessageToHapbeat(message);
      }
      lastColor = color;
    }

    StaticJsonDocument<200> doc;
    doc["r"] = r;
    doc["g"] = g;
    doc["b"] = b;
    String jsonMessage;
    serializeJson(doc, jsonMessage);

    MQTT_manager::sendMessageToWebApp(jsonMessage.c_str());
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

void TaskMQTT(void* args) {
  while (1) {
    MQTT_manager::loopMQTTclient();
    vTaskDelay(1 / portTICK_PERIOD_MS);
  }
}

void setup(void) {
  Serial.begin(115200);
#ifdef ESPNOW
  initEspNow();
#endif

#if defined(ENABLE_DISPLAY)
  initM5UImanager();
#endif

#ifdef ESPNOW
  espnowManager::init_esp_now(audioManager::PlaySndOnDataRecv);
#elif MQTT
  MQTT_manager::initMQTTclient(statusCallback);
#endif

#ifdef ENABLE_COLOR_SENSOR
  ColorSensor::initColorSensor();
  xTaskCreatePinnedToCore(TaskColorSensor, "TaskColorSensor", 4096, NULL, 0,
                          &thp[1], 0);
  xTaskCreatePinnedToCore(TaskMQTT, "TaskMQTT", 4096, NULL, 2, &thp[0], 1);
#endif
}

void loop(void) {
#ifdef ESPNOW
  sendSerialViaESPNOW();
#endif
  cmd_stat = "empty";
#if defined(ENABLE_DISPLAY)
  cmd_btn = M5ButtonNotify(cmd_stat);
  if (cmd_btn != "empty") {
    SendEspNOW(cmd_btn);
    Serial.println(cmd_btn);
  }
#endif
}

#if !defined(ARDUINO) && defined(ESP_PLATFORM)
extern "C" {
void loopTask(void*) {
  setup();
  for (;;) {
    loop();
  }
  vTaskDelete(NULL);
}

void app_main() {
  xTaskCreatePinnedToCore(loopTask, "loopTask", 8192, NULL, 1, NULL, 1);
}
}
#endif
