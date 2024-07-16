// #include <XcontrollerESP32.h>
#include <Arduino.h>

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

// cmd cmd_status of input
const char* cmd_stat;
const char* cmd_btn;
const char* cmd_conn;

void statusCallback(const char* status) { Serial.printf(status); }

//////////////////// general functions ///////////

TaskHandle_t thp[2];
void TaskColorSensor(void* args) {
  while (1) {
    ColorSensor::loopColorSensor();
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}
void TaskMQTT(void* args) {
  while (1) {

    vTaskDelay(1 / portTICK_PERIOD_MS);
  }
}

//////////////////// set up /////////////////////
void setup(void) {
  Serial.begin(115200);
  // initXconnESP32();
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
  xTaskCreatePinnedToCore(TaskColorSensor, "TaskColorSensor", 4096, NULL, 0, &thp[1], 0);
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

  // cmd_conn = recieveXconnNotify(cmd_stat);
  // if (cmd_conn != "empty") {
  //   SendEspNOW(cmd_conn);
  //   Serial.println(cmd_conn);
  // }
  // delay(1);
