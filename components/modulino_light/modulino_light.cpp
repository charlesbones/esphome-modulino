#include "modulino_light.h"
#include "esphome/core/log.h"

namespace esphome {
namespace modulino_light {

static const char *const TAG = "modulino_light";

void ModulinoLightComponent::setup() {
  // Bring the Arduino Wire bus up on the Modulino/Qwiic pins before the library
  // starts it (the library's own Wire.begin() then keeps these pins).
  Wire.begin(MODULINO_I2C_SDA, MODULINO_I2C_SCL);
  Modulino.begin();
  found_ = light_.begin();
  if (!found_) {
    ESP_LOGW(TAG, "Modulino Light not found on the I2C bus yet — will keep retrying");
  }
}

void ModulinoLightComponent::update() {
  if (!found_) {
    found_ = light_.begin();
    if (!found_) return;
    ESP_LOGI(TAG, "Modulino Light found");
  }
  if (!light_.update()) {
    ESP_LOGW(TAG, "Reading from Modulino Light failed");
    return;
  }
  if (illuminance_sensor_ != nullptr) {
    illuminance_sensor_->publish_state(light_.getLux());
  }
  if (ir_sensor_ != nullptr) {
    ir_sensor_->publish_state(light_.getIR());
  }
  if (red_sensor_ != nullptr || green_sensor_ != nullptr || blue_sensor_ != nullptr) {
    // ModulinoColor packs the channels as (r << 24) | (g << 16) | (b << 8).
    uint32_t packed = static_cast<uint32_t>(light_.getColor());
    if (red_sensor_ != nullptr) {
      red_sensor_->publish_state((packed >> 24) & 0xFF);
    }
    if (green_sensor_ != nullptr) {
      green_sensor_->publish_state((packed >> 16) & 0xFF);
    }
    if (blue_sensor_ != nullptr) {
      blue_sensor_->publish_state((packed >> 8) & 0xFF);
    }
  }
}

void ModulinoLightComponent::dump_config() {
  ESP_LOGCONFIG(TAG, "Modulino Light:");
  if (!found_) {
    ESP_LOGCONFIG(TAG, "  Communication with Modulino Light failed!");
  }
  LOG_SENSOR("  ", "Illuminance", illuminance_sensor_);
  LOG_SENSOR("  ", "IR", ir_sensor_);
  LOG_SENSOR("  ", "Red", red_sensor_);
  LOG_SENSOR("  ", "Green", green_sensor_);
  LOG_SENSOR("  ", "Blue", blue_sensor_);
}

}  // namespace modulino_light
}  // namespace esphome
