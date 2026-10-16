#include "modulino_thermo.h"
#include "esphome/core/log.h"

namespace esphome {
namespace modulino_thermo {

static const char *const TAG = "modulino_thermo";

void ModulinoThermoComponent::setup() {
  // Bring the Arduino Wire bus up on the Modulino/Qwiic pins before the library
  // starts it (the library's own Wire.begin() then keeps these pins).
  Wire.begin(MODULINO_I2C_SDA, MODULINO_I2C_SCL);
  Modulino.begin();
  found_ = thermo_.begin();
  if (!found_) {
    ESP_LOGW(TAG, "Modulino Thermo not found on the I2C bus yet — will keep retrying");
  }
}

void ModulinoThermoComponent::update() {
  if (!found_) {
    found_ = thermo_.begin();
    if (!found_) return;
    ESP_LOGI(TAG, "Modulino Thermo found");
  }
  if (temperature_sensor_ != nullptr) {
    temperature_sensor_->publish_state(thermo_.getTemperature());
  }
  if (humidity_sensor_ != nullptr) {
    humidity_sensor_->publish_state(thermo_.getHumidity());
  }
}

void ModulinoThermoComponent::dump_config() {
  ESP_LOGCONFIG(TAG, "Modulino Thermo:");
  if (!found_) {
    ESP_LOGCONFIG(TAG, "  Communication with Modulino Thermo failed!");
  }
  LOG_SENSOR("  ", "Temperature", temperature_sensor_);
  LOG_SENSOR("  ", "Humidity", humidity_sensor_);
}

}  // namespace modulino_thermo
}  // namespace esphome
