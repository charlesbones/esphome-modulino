#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/binary_sensor/binary_sensor.h"

#include <Arduino_Modulino.h>

// Thin wrapper around Arduino's own ModulinoJoystick class (2-axis analog
// joystick with a push button). Recycles the library's
// update()/getX()/getY()/isPressed() accessors.

namespace esphome {
namespace modulino_joystick {

class ModulinoJoystickComponent : public PollingComponent {
 public:
  void set_x_sensor(sensor::Sensor *sensor) { x_sensor_ = sensor; }
  void set_y_sensor(sensor::Sensor *sensor) { y_sensor_ = sensor; }
  void set_pressed_binary_sensor(binary_sensor::BinarySensor *binary_sensor) {
    pressed_binary_sensor_ = binary_sensor;
  }

  void setup() override;
  void update() override;
  void dump_config() override;

 protected:
  ModulinoJoystick joystick_;
  bool found_{false};
  sensor::Sensor *x_sensor_{nullptr};
  sensor::Sensor *y_sensor_{nullptr};
  binary_sensor::BinarySensor *pressed_binary_sensor_{nullptr};
};

}  // namespace modulino_joystick
}  // namespace esphome
