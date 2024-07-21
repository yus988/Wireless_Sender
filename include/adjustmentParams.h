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
    .rMin = 140, .rMax = 255, .gMin = 0, .gMax = 60, .bMin = 0, .bMax = 60};

const ColorThreshold BLUE_THD = {
    .rMin = 0, .rMax = 100, .gMin = 0, .gMax = 255, .bMin = 90, .bMax = 255};

const ColorThreshold YELLOW_THD = {
    .rMin = 110, .rMax = 255, .gMin = 80, .gMax = 255, .bMin = 0, .bMax = 50};

// RGB値を保持する構造体の定義
struct RGB {
  uint8_t r;
  uint8_t g;
  uint8_t b;
};

const RGB COLOR_UNCONNECTED = {10, 0, 0};
const RGB COLOR_CONNECTED = {0, 10, 0};
  #define CREATE_CRGB(color) CRGB((color).r, (color).g, (color).b)

  // Hapbeat用パラメータの定義（共通）
  #define CATEGORY 99
  #define WEARER_ID 99
  #define DEVICE_POS 99
  #define SUB_ID 0
  #define PLAY_CMD 1

// VibrationParams 構造体の定義
struct VibrationParams {
  uint8_t id;   // データの種類 0--
  uint8_t vol;  // 振動の強さ 0--255
};

// 色別のIDとボリュームのインスタンスを定義
const VibrationParams RED_PARAMS = {.id = 2, .vol = 30};
const VibrationParams GREEN_PARAMS = {.id = 5, .vol = 30};
const VibrationParams YELLOW_PARAMS = {.id = 4, .vol = 30};

#endif

#endif