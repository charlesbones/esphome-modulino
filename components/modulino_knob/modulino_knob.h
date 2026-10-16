#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/binary_sensor/binary_sensor.h"

#include <Arduino_Modulino.h>

// Thin wrapper around Arduino's own ModulinoKnob class (rotary encoder with a
// push button, I2C address 0x74/0x76). Recycles the library's get()/isPressed()
// accessors rather than re-deriving the encoder protocol.

namespace esphome {
namespace modulino_knob {

class ModulinoKnobComponent : public PollingComponent {
 public:
  void set_position_sensor(sensor::Sensor *sensor) { position_sensor_ = sensor; }
  void set_pressed_binary_sensor(binary_sensor::BinarySensor *binary_sensor) {
    pressed_binary_sensor_ = binary_sensor;
  }

  void setup() override;
  void update() override;
  void dump_config() override;

 protected:
  ModulinoKnob knob_;
  bool found_{false};
  sensor::Sensor *position_sensor_{nullptr};
  binary_sensor::BinarySensor *pressed_binary_sensor_{nullptr};
};

}  // namespace modulino_knob
}  // namespace esphome
