#include <Arduino.h>
#include "Adafruit_TCS34725.h"
#include "ColorSensor.h"

namespace ColorSensor {

Adafruit_TCS34725 tcs =
    Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_614MS, TCS34725_GAIN_1X);

void initColorSensor() {
  if (tcs.begin()) {
    Serial.println("Found sensor");
  } else {
    Serial.println("No TCS34725 found ... check your connections");
    while (1);
  }
}

void getColorValues(uint8_t& r, uint8_t& g, uint8_t& b) {
  uint16_t raw_r, raw_g, raw_b, c;
  tcs.getRawData(&raw_r, &raw_g, &raw_b, &c);

  // 輝度（クリアチャネル）を基に正規化
  float normalized_r = raw_r / (float)c;
  float normalized_g = raw_g / (float)c;
  float normalized_b = raw_b / (float)c;

  // 0-255の範囲に収める
  r = min(255, max(0, (int)(normalized_r * 255.0)));
  g = min(255, max(0, (int)(normalized_g * 255.0)));
  b = min(255, max(0, (int)(normalized_b * 255.0)));

  
}



void loopColorSensor() {
  uint16_t r, g, b, c, colorTemp, lux;
  tcs.getRawData(&r, &g, &b, &c);
  // colorTemp = tcs.calculateColorTemperature(r, g, b);
  colorTemp = tcs.calculateColorTemperature_dn40(r, g, b, c);
  lux = tcs.calculateLux(r, g, b);
  Serial.print("Color Temp: ");
  Serial.print(colorTemp, DEC);
  Serial.print(" K - ");
  Serial.print("Lux: ");
  Serial.print(lux, DEC);
  Serial.print(" - ");
  Serial.print("R: ");
  Serial.print(r, DEC);
  Serial.print(" ");
  Serial.print("G: ");
  Serial.print(g, DEC);
  Serial.print(" ");
  Serial.print("B: ");
  Serial.print(b, DEC);
  Serial.print(" ");
  Serial.print("C: ");
  Serial.print(c, DEC);
  Serial.print(" ");
  Serial.println(" ");
}

}  // namespace ColorSensor
