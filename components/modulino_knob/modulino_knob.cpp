#include "modulino_knob.h"
#include "esphome/core/log.h"

namespace esphome {
namespace modulino_knob {

static const char *const TAG = "modulino_knob";

void ModulinoKnobComponent::setup() {
  // Bring the Arduino Wire bus up on the Modulino/Qwiic pins before the library
  // starts it (the library's own Wire.begin() then keeps these pins).
  Wire.begin(MODULINO_I2C_SDA, MODULINO_I2C_SCL);
  Modulino.begin();
  found_ = knob_.begin();
  if (!found_) {
    ESP_LOGW(TAG, "Modulino Knob not found on the I2C bus yet — will keep retrying");
  }
}

void ModulinoKnobComponent::update() {
  if (!found_) {
    found_ = knob_.begin();
    if (!found_) return;
    ESP_LOGI(TAG, "Modulino Knob found");
  }
  if (position_sensor_ != nullptr) {
    position_sensor_->publish_state(knob_.get());
  }
  if (pressed_binary_sensor_ != nullptr) {
    pressed_binary_sensor_->publish_state(knob_.isPressed() != 0);
  }
}

void ModulinoKnobComponent::dump_config() {
  ESP_LOGCONFIG(TAG, "Modulino Knob:");
  if (!found_) {
    ESP_LOGCONFIG(TAG, "  Communication with Modulino Knob failed!");
  }
  LOG_SENSOR("  ", "Position", position_sensor_);
  LOG_BINARY_SENSOR("  ", "Pressed", pressed_binary_sensor_);
}

}  // namespace modulino_knob
}  // namespace esphome
