#include "modulino_buttons.h"
#include "esphome/core/log.h"

namespace esphome {
namespace modulino_buttons {

static const char *const TAG = "modulino_buttons";

void ModulinoButtonsComponent::setup() {
  // Bring the Arduino Wire bus up on the Modulino/Qwiic pins before the library
  // starts it (the library's own Wire.begin() then keeps these pins).
  Wire.begin(MODULINO_I2C_SDA, MODULINO_I2C_SCL);
  Modulino.begin();
  found_ = buttons_.begin();
  if (!found_) {
    ESP_LOGW(TAG, "Modulino Buttons not found on the I2C bus yet — will keep retrying");
  }
}

void ModulinoButtonsComponent::update() {
  if (!found_) {
    found_ = buttons_.begin();
    if (!found_) return;
    ESP_LOGI(TAG, "Modulino Buttons found");
  }
  // update() refreshes the cached button states (its bool return only signals
  // whether anything changed since the last poll).
  buttons_.update();
  if (button_a_ != nullptr) button_a_->publish_state(buttons_.isPressed(0) != 0);
  if (button_b_ != nullptr) button_b_->publish_state(buttons_.isPressed(1) != 0);
  if (button_c_ != nullptr) button_c_->publish_state(buttons_.isPressed(2) != 0);
}

void ModulinoButtonsComponent::dump_config() {
  ESP_LOGCONFIG(TAG, "Modulino Buttons:");
  if (!found_) {
    ESP_LOGCONFIG(TAG, "  Communication with Modulino Buttons failed!");
  }
  LOG_BINARY_SENSOR("  ", "Button A", button_a_);
  LOG_BINARY_SENSOR("  ", "Button B", button_b_);
  LOG_BINARY_SENSOR("  ", "Button C", button_c_);
}

}  // namespace modulino_buttons
}  // namespace esphome
