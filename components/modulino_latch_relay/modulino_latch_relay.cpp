#include "modulino_latch_relay.h"
#include "esphome/core/log.h"

namespace esphome {
namespace modulino_latch_relay {

static const char *const TAG = "modulino_latch_relay";

void ModulinoLatchRelaySwitch::setup() {
  // Bring the Arduino Wire bus up on the Modulino/Qwiic pins before the library
  // starts it (the library's own Wire.begin() then keeps these pins).
  Wire.begin(MODULINO_I2C_SDA, MODULINO_I2C_SCL);
  Modulino.begin();
  found_ = relay_.begin();
  if (!found_) {
    ESP_LOGW(TAG, "Modulino Latch Relay not found on the I2C bus yet — will retry on first use");
  }
}

void ModulinoLatchRelaySwitch::write_state(bool state) {
  if (!found_) {
    found_ = relay_.begin();
    if (!found_) {
      ESP_LOGW(TAG, "Modulino Latch Relay not found; ignoring command");
      return;
    }
  }
  if (state) {
    relay_.set();
  } else {
    relay_.reset();
  }
  this->publish_state(state);
}

void ModulinoLatchRelaySwitch::dump_config() {
  ESP_LOGCONFIG(TAG, "Modulino Latch Relay:");
  if (!found_) {
    ESP_LOGCONFIG(TAG, "  Communication with Modulino Latch Relay failed!");
  }
}

}  // namespace modulino_latch_relay
}  // namespace esphome
