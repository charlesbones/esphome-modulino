#include "modulino_buzzer.h"
#include "esphome/core/log.h"

namespace esphome {
namespace modulino_buzzer {

static const char *const TAG = "modulino_buzzer";

// The buzzer firmware plays a tone for a fixed duration; use the maximum so a
// switched-on tone is effectively continuous until write_state(false).
static const uint32_t CONTINUOUS_MS = 0xFFFFFFFFUL;

void ModulinoBuzzerSwitch::setup() {
  // Bring the Arduino Wire bus up on the Modulino/Qwiic pins before the library
  // starts it (the library's own Wire.begin() then keeps these pins).
  Wire.begin(MODULINO_I2C_SDA, MODULINO_I2C_SCL);
  Modulino.begin();
  found_ = buzzer_.begin();
  if (!found_) {
    ESP_LOGW(TAG, "Modulino Buzzer not found on the I2C bus yet — will retry on first use");
  }
}

void ModulinoBuzzerSwitch::write_state(bool state) {
  if (!found_) {
    found_ = buzzer_.begin();
    if (!found_) {
      ESP_LOGW(TAG, "Modulino Buzzer not found; ignoring command");
      return;
    }
  }
  // Parenthesized calls: tone()/noTone() may be function-like macros under the
  // Arduino core, which the member call would otherwise trip over.
  if (state) {
    (buzzer_.tone)(frequency_, CONTINUOUS_MS);
  } else {
    (buzzer_.noTone)();
  }
  this->publish_state(state);
}

void ModulinoBuzzerSwitch::dump_config() {
  ESP_LOGCONFIG(TAG, "Modulino Buzzer:");
  ESP_LOGCONFIG(TAG, "  Frequency: %u Hz", frequency_);
  if (!found_) {
    ESP_LOGCONFIG(TAG, "  Communication with Modulino Buzzer failed!");
  }
}

}  // namespace modulino_buzzer
}  // namespace esphome
