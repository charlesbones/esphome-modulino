#include "modulino_vibro.h"
#include "esphome/core/log.h"

namespace esphome {
namespace modulino_vibro {

static const char *const TAG = "modulino_vibro";

// The vibro firmware runs for a fixed duration; use the maximum so a
// switched-on motor runs effectively continuously until write_state(false).
static const uint32_t CONTINUOUS_MS = 0xFFFFFFFFUL;

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

void ModulinoVibroSwitch::write_state(bool state) {
  if (!found_) {
    found_ = vibro_.begin();
    if (!found_) {
      ESP_LOGW(TAG, "Modulino Vibro not found; ignoring command");
      return;
    }
  }
  if (state) {
    vibro_.on(CONTINUOUS_MS, false, MAXIMUM);
  } else {
    vibro_.off();
  }
  this->publish_state(state);
}

void ModulinoVibroSwitch::dump_config() {
  ESP_LOGCONFIG(TAG, "Modulino Vibro:");
  if (!found_) {
    ESP_LOGCONFIG(TAG, "  Communication with Modulino Vibro failed!");
  }
}

}  // namespace modulino_vibro
}  // namespace esphome
