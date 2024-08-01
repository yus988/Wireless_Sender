// 頻繁に調整したい変数はこちらで定義
#ifndef ADJ_PARAMS_H
#define ADJ_PARAMS_H

#ifdef ENABLE_COLOR_SENSOR
//   #include <FastLED.h>
// カラーコンフィグ
// 各色のしきい値を定義（例）
struct ColorThreshold {
  uint8_t rMin;
  uint8_t rMax;
  uint8_t gMin;
  uint8_t gMax;
  uint8_t bMin;
  uint8_t bMax;
};

// 各色のしきい値のインスタンスを定義
const ColorThreshold RED_THD = {
    .rMin = 160, .rMax = 255, .gMin = 0, .gMax = 50, .bMin = 0, .bMax = 70};

const ColorThreshold BLUE_THD = {
    .rMin = 0, .rMax = 20, .gMin = 0, .gMax = 80, .bMin = 180, .bMax = 255};

const ColorThreshold YELLOW_THD = {
    .rMin = 130, .rMax = 159, .gMin = 50, .gMax = 80, .bMin = 0, .bMax = 40};

// VibrationParams 構造体の定義
struct VibrationParams {
  uint8_t id;   // データの種類 0--
  uint8_t vol;  // 振動の強さ 0--255
};
// 色別のIDとボリュームのインスタンスを定義
const VibrationParams BLUE_PARAMS = {.id = 0, .vol = 50};
const VibrationParams YELLOW_PARAMS = {.id = 1, .vol = 50};
const VibrationParams RED_PARAMS = {.id = 2, .vol = 50};

// RGB値を保持する構造体の定義
struct RGB {
  uint8_t r;
  uint8_t g;
  uint8_t b;
};
// 接続 / 切断時のLEDの色
const RGB COLOR_CONNECTED = {0, 255, 0};
const RGB COLOR_UNCONNECTED = {255, 0, 0};
  #define CREATE_CRGB(color) CRGB((color).r, (color).g, (color).b)

  // H/W 設定
  #define LED_BRIGHTNESS 5  // 0--255

  // ループ時間の変数
  #define SEND_WEBAPP_INTERVAL \
    1000  // ミリ秒ごとに実行。webappに送信するインターバル
  #define COLOR_SENSOR_INTERVAL \
    50  // ミリ秒、TaskColorSensorを実行するインターバル
  #define COLOR_CHANGE_INTERVAL \
    5000  // ミリ秒、同じ色で再度有効にするためのインターバル

  #define RETAIN_REFRESH_INTERVAL \
    60000  // ミリ秒、retatin で空のメッセージを送るインターバル

  // Hapbeat用パラメータの定義（共通）
  #define CATEGORY 99
  #define WEARER_ID 99
  #define DEVICE_POS 99
  #define SUB_ID 0
  #define PLAY_CMD 1  // 0: oneshot, 1: loopStart, 2: loopStop

#endif

#endif