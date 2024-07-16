#ifndef COLOR_SENSOR_H
  #define SENSORS_H

  #ifdef ENABLE_COLOR_SENSOR
namespace ColorSensor {
void initColorSensor();
void loopColorSensor();
}  // namespace ColorSensor
  #endif

#endif