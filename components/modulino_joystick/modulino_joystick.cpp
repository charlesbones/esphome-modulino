#include "modulino_joystick.h"
#include "esphome/core/log.h"

namespace esphome {
namespace modulino_joystick {

static const char *const TAG = "modulino_joystick";

void ModulinoJoystickComponent::setup() {
  // Bring the Arduino Wire bus up on the Modulino/Qwiic pins before the library
  // starts it (the library's own Wire.begin() then keeps these pins).
  Wire.begin(MODULINO_I2C_SDA, MODULINO_I2C_SCL);
  Modulino.begin();
  found_ = joystick_.begin();
  if (!found_) {
    ESP_LOGW(TAG, "Modulino Joystick not found on the I2C bus yet — will keep retrying");
  }
}

void ModulinoJoystickComponent::update() {
  if (!found_) {
    found_ = joystick_.begin();
    if (!found_) return;
    ESP_LOGI(TAG, "Modulino Joystick found");
  }
  // update() refreshes the cached axis/button state (its bool return only
  // signals whether anything changed since the last poll).
  joystick_.update();
  if (x_sensor_ != nullptr) x_sensor_->publish_state(joystick_.getX());
  if (y_sensor_ != nullptr) y_sensor_->publish_state(joystick_.getY());
  if (pressed_binary_sensor_ != nullptr) {
    pressed_binary_sensor_->publish_state(joystick_.isPressed() != 0);
  }
}

void ModulinoJoystickComponent::dump_config() {
  ESP_LOGCONFIG(TAG, "Modulino Joystick:");
  if (!found_) {
    ESP_LOGCONFIG(TAG, "  Communication with Modulino Joystick failed!");
  }
  LOG_SENSOR("  ", "X", x_sensor_);
  LOG_SENSOR("  ", "Y", y_sensor_);
  LOG_BINARY_SENSOR("  ", "Pressed", pressed_binary_sensor_);
}

}  // namespace modulino_joystick
}  // namespace esphome
