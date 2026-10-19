#include "modulino_buzzer.h"
#include "esphome/core/log.h"

namespace esphome {
namespace modulino_buzzer {

static const char *const TAG = "modulino_buzzer";

// The buzzer firmware plays a tone only for a bounded duration (its own examples
// use values around 1000 ms; very large values are rejected). To make a
// switched-on tone continuous we re-send it before the previous one elapses:
// the retrigger interval is shorter than the tone duration, so there is no gap.
static const uint32_t TONE_DURATION_MS = 1000;
static const uint32_t RETRIGGER_INTERVAL_MS = 500;

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

bool ModulinoBuzzerSwitch::ensure_found_() {
  if (!found_) {
    found_ = buzzer_.begin();
    if (found_) ESP_LOGI(TAG, "Modulino Buzzer found");
  }
  return found_;
}

void ModulinoBuzzerSwitch::write_state(bool state) {
  if (!ensure_found_()) {
    ESP_LOGW(TAG, "Modulino Buzzer not found; ignoring command");
    return;
  }
  // Parenthesized calls: tone()/noTone() may be function-like macros under the
  // Arduino core, which the member call would otherwise trip over.
  if (state) {
    (buzzer_.tone)(frequency_, TONE_DURATION_MS);
    last_trigger_ = millis();
  } else {
    (buzzer_.noTone)();
  }
  active_ = state;
  this->publish_state(state);
}

void ModulinoBuzzerSwitch::loop() {
  if (!active_ || !found_) return;
  if (millis() - last_trigger_ >= RETRIGGER_INTERVAL_MS) {
    (buzzer_.tone)(frequency_, TONE_DURATION_MS);
    last_trigger_ = millis();
  }
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
