#include "modulino_distance.h"
#include "esphome/core/log.h"

namespace esphome {
namespace modulino_distance {

static const char *const TAG = "modulino_distance";

void ModulinoDistanceComponent::setup() {
  // Bring the Arduino Wire bus up on the Modulino/Qwiic pins before the library
  // starts it (the library's own Wire.begin() then keeps these pins).
  Wire.begin(MODULINO_I2C_SDA, MODULINO_I2C_SCL);
  Modulino.begin();
  found_ = distance_.begin();
  if (!found_) {
    ESP_LOGW(TAG, "Modulino Distance not found on the I2C bus yet — will keep retrying");
  }
}

void ModulinoDistanceComponent::update() {
  if (!found_) {
    found_ = distance_.begin();
    if (!found_) return;
    ESP_LOGI(TAG, "Modulino Distance found");
  }
  // Refresh the reading, then publish it unconditionally: get() is NAN when
  // nothing is in range, which Home Assistant shows as "unknown" rather than a
  // stale distance.
  distance_.available();
  this->publish_state(distance_.get());
}

void ModulinoDistanceComponent::dump_config() {
  LOG_SENSOR("", "Modulino Distance", this);
  if (!found_) {
    ESP_LOGCONFIG(TAG, "  Communication with Modulino Distance failed!");
  }
}

}  // namespace modulino_distance
}  // namespace esphome
