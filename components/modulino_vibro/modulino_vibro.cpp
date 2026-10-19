#include "modulino_vibro.h"
#include "esphome/core/log.h"

namespace esphome {
namespace modulino_vibro {

static const char *const TAG = "modulino_vibro";

// The vibro firmware runs the motor only for a bounded duration (its own
// examples use ~1000 ms; very large values are rejected). For continuous mode
// (duration = 0) we re-send the command before the previous run elapses; the
// retrigger interval is shorter than the run chunk, so there is no gap.
static const uint32_t CONTINUOUS_CHUNK_MS = 1000;
static const uint32_t RETRIGGER_INTERVAL_MS = 500;

void ModulinoVibroSwitch::setup() {
  // Bring the Arduino Wire bus up on the Modulino/Qwiic pins before the library
  // starts it (the library's own Wire.begin() then keeps these pins).
  Wire.begin(MODULINO_I2C_SDA, MODULINO_I2C_SCL);
  Modulino.begin();
  found_ = vibro_.begin();
  if (!found_) {
    ESP_LOGW(TAG, "Modulino Vibro not found on the I2C bus yet — will retry on first use");
  }
}

bool ModulinoVibroSwitch::ensure_found_() {
  if (!found_) {
    found_ = vibro_.begin();
    if (found_) ESP_LOGI(TAG, "Modulino Vibro found");
  }
  return found_;
}

void ModulinoVibroSwitch::set_intensity(float percent) {
  if (percent < 0.0f) percent = 0.0f;
  if (percent > 100.0f) percent = 100.0f;
  intensity_ = percent;
}

int ModulinoVibroSwitch::power_() const {
  // Map 0..100 % onto 0..MAXIMUM, the library's strongest level.
  return static_cast<int>(intensity_ / 100.0f * static_cast<float>(MAXIMUM));
}

void ModulinoVibroSwitch::write_state(bool state) {
  if (!ensure_found_()) {
    ESP_LOGW(TAG, "Modulino Vibro not found; ignoring command");
    return;
  }
  if (state) {
    uint32_t chunk = duration_ms_ > 0 ? duration_ms_ : CONTINUOUS_CHUNK_MS;
    if (power_() > 0) vibro_.on(chunk, false, power_());  // 0 % intensity = no motion; continuous mode picks it up on the next retrigger
    on_since_ = millis();
    last_trigger_ = on_since_;
  } else {
    vibro_.off();
  }
  active_ = state;
  this->publish_state(state);
}

void ModulinoVibroSwitch::loop() {
  if (!active_ || !found_) return;
  uint32_t now = millis();
  if (duration_ms_ > 0) {
    // Timed pulse: the motor stops on its own; return the switch to off.
    if (now - on_since_ >= duration_ms_) {
      vibro_.off();
      active_ = false;
      this->publish_state(false);
    }
  } else {
    // Continuous: re-send before the current chunk elapses.
    if (now - last_trigger_ >= RETRIGGER_INTERVAL_MS) {
      vibro_.on(CONTINUOUS_CHUNK_MS, false, power_());
      last_trigger_ = now;
    }
  }
}

void ModulinoVibroSwitch::dump_config() {
  ESP_LOGCONFIG(TAG, "Modulino Vibro:");
  if (!found_) {
    ESP_LOGCONFIG(TAG, "  Communication with Modulino Vibro failed!");
  }
}

void ModulinoVibroNumber::control(float value) {
  if (type_ == ModulinoVibroNumberType::INTENSITY) {
    this->parent_->set_intensity(value);
  } else {
    this->parent_->set_duration(static_cast<uint32_t>(value));
  }
  this->publish_state(value);
}

}  // namespace modulino_vibro
}  // namespace esphome
