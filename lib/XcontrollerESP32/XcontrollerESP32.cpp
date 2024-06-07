#include <XboxSeriesXControllerESP32_asukiaaa.hpp>
// https://asukiaaa.blogspot.com/2021/01/communicate-esp32-and-xbox-controller-on-ble.html

// controller addr
XboxSeriesXControllerESP32_asukiaaa::Core xboxController("14:cb:65:92:6f:3c");
// XboxSeriesXControllerESP32_asukiaaa::Core
// xboxController("G8:C3:CC:5A:09:00"); // iPhone addr

// any xbox controller
// XboxSeriesXControllerESP32_asukiaaa::Core xboxController;

void initXconnESP32() { xboxController.begin(); }

// recieve notify and store data to variables
/* xboxNotif.toString()で表示される内容
--------------------------------------------------------
btnY: 0 btnX: 0 btnB: 0 btnA: 0 btnLB: 0 btnRB: 0
btnSelect: 0 btnStart: 0 btnXbox: 0 btnShare: 0 btnLS: 0 btnRS: 0
btnDirUp: 0 btnDirRight: 0 btnDirDown: 0 btnDirLeft: 0
joyLHori: 33261
joyLVert: 32713
joyRHori: 32387
joyRVert: 32550
trigLT: 979
trigRT: 0
---------------------------------------------------------
*/
// clang-format off
bool btnStat[] = {
 false, false, false, false, false, false,
 false, false, false, false, false, false, 
 false, false, false, false};

const char *statTxt[] = {
  "btnY", "btnX", "btnB", "btnA", "btnLB", "btnRB",
  "btnSelect", "btnStart", "btnXbox", "btnShare", "btnLS", "btnRS",
  "btnDirUp", "btnDirRight", "btnDirDown", "btnDirLeft"
};
// clang-format on

const char *recieveXconnNotify(const char *stat) {
  stat = stat;
  xboxController.onLoop();
  if (xboxController.isConnected()) {
    if (xboxController.isWaitingForFirstNotification()) {
      Serial.println("waiting for first notification");
    } else {
      //   Serial.println("Address: " + xboxController.buildDeviceAddressStr());
      // Serial.print(xboxController.xboxNotif.toString());
      //   uint16_t joystickMax = XboxControllerNotificationParser::maxJoy;
      //   Serial.print("joyLHori rate: ");
      //   Serial.println((float)xboxController.xboxNotif.joyLHori /
      //   joystickMax); Serial.print("joyLVert rate: ");
      //   Serial.println((float)xboxController.xboxNotif.joyLVert /
      //   joystickMax); Serial.println("battery " +
      //   String(xboxController.battery) + "%"); unsigned long receivedAt =
      //   xboxController.getReceiveNotificationAt(); Serial.println("received
      //   at " + String(receivedAt));
      bool notifyBtnStat[] = {
          xboxController.xboxNotif.btnY, xboxController.xboxNotif.btnX,
          xboxController.xboxNotif.btnB, xboxController.xboxNotif.btnA,
          xboxController.xboxNotif.btnLB, xboxController.xboxNotif.btnRB,
          xboxController.xboxNotif.btnSelect, xboxController.xboxNotif.btnStart,
          xboxController.xboxNotif.btnXbox, xboxController.xboxNotif.btnShare,
          xboxController.xboxNotif.btnLS, xboxController.xboxNotif.btnRS,
          xboxController.xboxNotif.btnDirUp, xboxController.xboxNotif.btnDirRight, 
          xboxController.xboxNotif.btnDirDown, xboxController.xboxNotif.btnDirLeft, 
          };

      for (int i = 0; i < sizeof(btnStat) / sizeof(btnStat[0]); i++) {
        if (!btnStat[i] && notifyBtnStat[i]) {
          stat = statTxt[i];
          btnStat[i] = true;
        }
        if (btnStat[i] && !notifyBtnStat[i]) {
          btnStat[i] = false;
        }
      }
    }
  } else {
    /*
libdeps以下、XboxSeriesXControllerESP32_asukiaaa.hpp
uint32_t scanTime = 0;
にすることで、都度ESPのリスタートをしなくても良いかも？（要検証）
*/
    // Serial.println("not connected");
    // if (xboxController.getCountFailedConnection() > 2) {
    //   ESP.restart();
    // }
  }

  return stat;
}