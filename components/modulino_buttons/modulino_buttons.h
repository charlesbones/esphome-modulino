#pragma once

#include "esphome/core/component.h"
#include "esphome/components/binary_sensor/binary_sensor.h"

#include <Arduino_Modulino.h>

// Thin wrapper around Arduino's own ModulinoButtons class (three capacitive
// buttons, I2C address 0x7C). Recycles the library's update()/isPressed()
// accessors.

namespace esphome {
namespace modulino_buttons {

class ModulinoButtonsComponent : public PollingComponent {
 public:
  void set_button_a_binary_sensor(binary_sensor::BinarySensor *bs) { button_a_ = bs; }
  void set_button_b_binary_sensor(binary_sensor::BinarySensor *bs) { button_b_ = bs; }
  void set_button_c_binary_sensor(binary_sensor::BinarySensor *bs) { button_c_ = bs; }

  void setup() override;
  void update() override;
  void dump_config() override;

 protected:
  ModulinoButtons buttons_;
  bool found_{false};
  binary_sensor::BinarySensor *button_a_{nullptr};
  binary_sensor::BinarySensor *button_b_{nullptr};
  binary_sensor::BinarySensor *button_c_{nullptr};
};

}  // namespace modulino_buttons
}  // namespace esphome
