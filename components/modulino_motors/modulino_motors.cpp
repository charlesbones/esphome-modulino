#include "modulino_motors.h"
#include "esphome/core/log.h"

namespace esphome {
namespace modulino_motors {

static const char *const TAG = "modulino_motors";

void ModulinoMotorsComponent::apply_settings_() {
  if (has_decay_) motors_.setDecay(decay_);
  if (has_pwm_frequency_) motors_.setFrequency(pwm_frequency_);
  if (has_half_step_) motors_.setHalfStepEnabled(half_step_);
  motors_.setStepperModeEnabled(stepper_mode_);
}

void ModulinoMotorsComponent::setup() {
  // Bring the Arduino Wire bus up on the Modulino/Qwiic pins before the library
  // starts it (the library's own Wire.begin() then keeps these pins).
  Wire.begin(MODULINO_I2C_SDA, MODULINO_I2C_SCL);
  Modulino.begin();
  found_ = motors_.begin();
  if (found_) {
    apply_settings_();
  } else {
    ESP_LOGW(TAG, "Modulino Motors not found on the I2C bus yet — will keep retrying");
  }
}

void ModulinoMotorsComponent::update() {
  if (!found_) {
    found_ = motors_.begin();
    if (!found_) return;
    ESP_LOGI(TAG, "Modulino Motors found");
    apply_settings_();
    motors_.setDcSpeedRaw(raw_a_, raw_b_);  // apply speeds set before the node appeared
  }
  if (!motors_.update()) return;
  if (current_a_sensor_ != nullptr) {
    current_a_sensor_->publish_state(motors_.sensedCurrentA());
  }
  if (current_b_sensor_ != nullptr) {
    current_b_sensor_->publish_state(motors_.sensedCurrentB());
  }
  if (busy_binary_sensor_ != nullptr) {
    busy_binary_sensor_->publish_state(motors_.busy());
  }
}

void ModulinoMotorsComponent::set_channel_speed(bool is_b, float percent) {
  if (percent > 100.0f) percent = 100.0f;
  if (percent < -100.0f) percent = -100.0f;
  int16_t raw = static_cast<int16_t>(percent / 100.0f * ModulinoMotors::MAX_SPEED);
  if (is_b) {
    raw_b_ = raw;
  } else {
    raw_a_ = raw;
  }
  if (found_) {
    motors_.setDcSpeedRaw(raw_a_, raw_b_);
  }
}

bool ModulinoMotorsComponent::move_stepper(int32_t steps, uint16_t speed_period) {
  if (!found_) return false;
  return motors_.moveStepper(steps, speed_period);
}

void ModulinoMotorsComponent::dump_config() {
  ESP_LOGCONFIG(TAG, "Modulino Motors:");
  ESP_LOGCONFIG(TAG, "  Mode: %s", stepper_mode_ ? "stepper" : "dc");
  if (!found_) {
    ESP_LOGCONFIG(TAG, "  Communication with Modulino Motors failed!");
  }
  LOG_SENSOR("  ", "Current A", current_a_sensor_);
  LOG_SENSOR("  ", "Current B", current_b_sensor_);
  LOG_BINARY_SENSOR("  ", "Busy", busy_binary_sensor_);
}

void ModulinoMotorsNumber::control(float value) {
  this->parent_->set_channel_speed(is_b_, value);
  this->publish_state(value);
}

void ModulinoMotorsStepper::loop() {
  if (!this->parent_->is_found()) return;
  if (this->has_reached_target()) return;
  int32_t delta = this->target_position - this->current_position;
  // max_speed_ is in steps/s; the firmware paces steps by a period expressed in
  // 0.1 ms ticks, so ticks = 10000 / (steps/s).
  float speed = this->max_speed_;
  uint32_t period = (speed > 0.0f) ? static_cast<uint32_t>(10000.0f / speed) : 1;
  if (period < 1) period = 1;
  if (period > 65535) period = 65535;
  if (this->parent_->move_stepper(delta, static_cast<uint16_t>(period))) {
    // The move executes autonomously on the module; treat the target as reached.
    this->current_position = this->target_position;
  }
}

void ModulinoMotorsStepper::dump_config() {
  ESP_LOGCONFIG(TAG, "Modulino Motors Stepper:");
  ESP_LOGCONFIG(TAG, "  Max Speed: %.0f steps/s", this->max_speed_);
  ESP_LOGCONFIG(TAG, "  (acceleration is not supported by the Modulino firmware)");
}

}  // namespace modulino_motors
}  // namespace esphome
