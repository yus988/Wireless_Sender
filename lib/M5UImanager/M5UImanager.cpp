#include <M5Unified.h>

int screen_width;
int screen_height;
int button_width;
int button_height;
int buttonA_x;
int buttonA_y;
int buttonB_x;
int buttonB_y;
int buttonC_x;
int buttonC_y;

#define data_A {0, 0, 0, 0, 0, 150, 150, 0}
#define data_B {0, 0, 1, 0, 0, 150, 150, 0}
#define data_C {0, 0, 2, 0, 0, 200, 200, 0}

bool buttonA_pressed = false;
bool buttonB_pressed = false;
bool buttonC_pressed = false;

#ifdef M5STACK_GRAY

const char *M5ButtonNotify(const char *stat) {
  M5.update();
  static constexpr const int colors[] = {TFT_WHITE,  TFT_CYAN, TFT_RED,
                                         TFT_YELLOW, TFT_BLUE, TFT_GREEN};
  static constexpr const char *const names[] = {
      "none",       "wasHold",     "wasClicked",
      "wasPressed", "wasReleased", "wasDeciedCount"};

  int w = M5.Display.width() / 5;
  int h = M5.Display.height();
  M5.Display.startWrite();

  int state = M5.BtnA.wasHold()               ? 1
              : M5.BtnA.wasClicked()          ? 2
              : M5.BtnA.wasPressed()          ? 3
              : M5.BtnA.wasReleased()         ? 4
              : M5.BtnA.wasDecideClickCount() ? 5
                                              : 0;
  if (state) {
    if (state == 3) {
      stat = "BtnA";
    }
    M5.Display.fillRect(w * 1, 0, w - 1, h, colors[state]);
  }

  state = M5.BtnB.wasHold()               ? 1
          : M5.BtnB.wasClicked()          ? 2
          : M5.BtnB.wasPressed()          ? 3
          : M5.BtnB.wasReleased()         ? 4
          : M5.BtnB.wasDecideClickCount() ? 5
                                          : 0;
  if (state) {
    if (state == 3) {
      stat = "BtnB";
    }
    M5.Display.fillRect(w * 2, 0, w - 1, h, colors[state]);
  }

  state = M5.BtnC.wasHold()               ? 1
          : M5.BtnC.wasClicked()          ? 2
          : M5.BtnC.wasPressed()          ? 3
          : M5.BtnC.wasReleased()         ? 4
          : M5.BtnC.wasDecideClickCount() ? 5
                                          : 0;
  if (state) {
    if (state == 3) {
      stat = "BtnC";
    }
    M5.Display.fillRect(w * 3, 0, w - 1, h, colors[state]);
  }
  M5.Display.endWrite();
  return stat;
}

#endif

#ifdef M5STACK_CORE_S3

void drawButtons() {
  M5.Display.fillRect(buttonA_x, buttonA_y, button_width, button_height,
                      TFT_WHITE);
  M5.Display.fillRect(buttonB_x, buttonB_y, button_width, button_height,
                      TFT_WHITE);
  M5.Display.fillRect(buttonC_x, buttonC_y, button_width, button_height,
                      TFT_WHITE);

  // 境目に縦棒を描画
  M5.Display.fillRect(buttonB_x - 1, buttonB_y, 2, button_height, TFT_BLACK);
  M5.Display.fillRect(buttonC_x - 1, buttonC_y, 2, button_height, TFT_BLACK);

  M5.Display.setTextColor(TFT_BLACK);
  M5.Display.setTextDatum(CC_DATUM);

  M5.Display.drawString("Button A", buttonA_x + button_width / 2,
                        buttonA_y + button_height / 2);
  M5.Display.drawString("Button B", buttonB_x + button_width / 2,
                        buttonB_y + button_height / 2);
  M5.Display.drawString("Button C", buttonC_x + button_width / 2,
                        buttonC_y + button_height / 2);
}

const char *M5ButtonNotify(const char *stat) {
  M5.update();
  bool screenUpdated = false;

  if (M5.Touch.getCount() > 0) {
    auto touch = M5.Touch.getDetail();
    int touch_x = touch.x;
    int touch_y = touch.y;

    // ボタンAのタッチエリア判定
    if (touch_x > buttonA_x && touch_x < buttonA_x + button_width &&
        touch_y > buttonA_y && touch_y < buttonA_y + button_height) {
      if (!buttonA_pressed) {
        stat = "BtnA";
        screenUpdated = true;
      }
      buttonA_pressed = true;
      M5.Display.fillRect(buttonA_x, buttonA_y, button_width, button_height,
                          TFT_RED);
    } else {
      if (buttonA_pressed) {
        buttonA_pressed = false;
        screenUpdated = true;
      }
    }

    // ボタンBのタッチエリア判定
    if (touch_x > buttonB_x && touch_x < buttonB_x + button_width &&
        touch_y > buttonB_y && touch_y < buttonB_y + button_height) {
      if (!buttonB_pressed) {
        stat = "BtnB";
        screenUpdated = true;
      }
      buttonB_pressed = true;
      M5.Display.fillRect(buttonB_x, buttonB_y, button_width, button_height,
                          TFT_GREEN);
    } else {
      if (buttonB_pressed) {
        buttonB_pressed = false;
        screenUpdated = true;
      }
    }

    // ボタンCのタッチエリア判定
    if (touch_x > buttonC_x && touch_x < buttonC_x + button_width &&
        touch_y > buttonC_y && touch_y < buttonC_y + button_height) {
      if (!buttonC_pressed) {
        stat = "BtnC";
        screenUpdated = true;
      }
      buttonC_pressed = true;
      M5.Display.fillRect(buttonC_x, buttonC_y, button_width, button_height,
                          TFT_BLUE);
    } else {
      if (buttonC_pressed) {
        buttonC_pressed = false;
        screenUpdated = true;
      }
    }
  } else {
    // タッチされていないときに表示を元に戻す
    if (buttonA_pressed || buttonB_pressed || buttonC_pressed) {
      buttonA_pressed = false;
      buttonB_pressed = false;
      buttonC_pressed = false;
      screenUpdated = true;
    }
  }

  if (screenUpdated) {
    drawButtons();
  }

  return stat;
}

#endif

void initM5UImanager(void) {
  auto cfg = M5.config();
  cfg.external_spk = false;  // 内蔵スピーカーを無効にする例（必要に応じて設定）
  M5.begin(cfg);
  M5.Display.setTextSize(4);

#ifdef M5STACK_CORE_S3
  M5.Display.setTextSize(1);

  screen_width = M5.Display.width();
  screen_height = M5.Display.height();

  button_width = screen_width / 3;
  button_height = screen_height;

  buttonA_x = 0;
  buttonA_y = 0;

  buttonB_x = button_width;
  buttonB_y = 0;

  buttonC_x = 2 * button_width;
  buttonC_y = 0;

  drawButtons();
#endif
}
