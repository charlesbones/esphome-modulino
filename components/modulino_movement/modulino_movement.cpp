#include "modulino_movement.h"
#include "esphome/core/log.h"

namespace esphome {
namespace modulino_movement {

static const char *const TAG = "modulino_movement";

void ModulinoMovementComponent::setup() {
  // Bring the Arduino Wire bus up on the Modulino/Qwiic pins before the library
  // starts it (the library's own Wire.begin() then keeps these pins).
  Wire.begin(MODULINO_I2C_SDA, MODULINO_I2C_SCL);
  Modulino.begin();
  found_ = movement_.begin();
  if (!found_) {
    ESP_LOGW(TAG, "Modulino Movement not found on the I2C bus yet — will keep retrying");
  }
}

void ModulinoMovementComponent::update() {
  if (!found_) {
    found_ = movement_.begin();
    if (!found_) return;
    ESP_LOGI(TAG, "Modulino Movement found");
  }
  if (!movement_.update()) return;  // skip stale values when the read fails
  if (accel_x_sensor_ != nullptr) accel_x_sensor_->publish_state(movement_.getX());
  if (accel_y_sensor_ != nullptr) accel_y_sensor_->publish_state(movement_.getY());
  if (accel_z_sensor_ != nullptr) accel_z_sensor_->publish_state(movement_.getZ());
  if (gyro_x_sensor_ != nullptr) gyro_x_sensor_->publish_state(movement_.getRoll());
  if (gyro_y_sensor_ != nullptr) gyro_y_sensor_->publish_state(movement_.getPitch());
  if (gyro_z_sensor_ != nullptr) gyro_z_sensor_->publish_state(movement_.getYaw());
}

void ModulinoMovementComponent::dump_config() {
  ESP_LOGCONFIG(TAG, "Modulino Movement:");
  if (!found_) {
    ESP_LOGCONFIG(TAG, "  Communication with Modulino Movement failed!");
  }
  LOG_SENSOR("  ", "Acceleration X", accel_x_sensor_);
  LOG_SENSOR("  ", "Acceleration Y", accel_y_sensor_);
  LOG_SENSOR("  ", "Acceleration Z", accel_z_sensor_);
  LOG_SENSOR("  ", "Gyroscope X", gyro_x_sensor_);
  LOG_SENSOR("  ", "Gyroscope Y", gyro_y_sensor_);
  LOG_SENSOR("  ", "Gyroscope Z", gyro_z_sensor_);
}

}  // namespace modulino_movement
}  // namespace esphome
