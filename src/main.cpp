#include <espnow.h>
#include <XcontrollerESP32.h>
#include <Arduino.h>

#if defined(M5STACK_GRAY) || defined(M5STACK_CORE_S3)
  #include <M5UImanager.h>
#endif


// cmd cmd_status of input
const char* cmd_stat;
const char* cmd_btn;
const char* cmd_conn;

void setup(void) {
  Serial.begin(115200);
  // initXconnESP32();
  initEspNow();
#if defined(M5STACK_GRAY) || defined(M5STACK_CORE_S3)
  initM5UImanager();
#endif
}

void loop(void) {
  sendSerialViaESPNOW();
  cmd_stat = "empty";
#if defined(M5STACK_GRAY) || defined(M5STACK_CORE_S3)
  cmd_btn = M5ButtonNotify(cmd_stat);
  if (cmd_btn != "empty") {
    SendEspNOW(cmd_btn);
    Serial.println(cmd_btn);
  }
#endif
  // cmd_conn = recieveXconnNotify(cmd_stat);
  // if (cmd_conn != "empty") {
  //   SendEspNOW(cmd_conn);
  //   Serial.println(cmd_conn);
  // }
  // delay(1);
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