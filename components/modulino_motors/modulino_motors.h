#pragma once

#include "esphome/core/component.h"
#include "esphome/core/helpers.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/binary_sensor/binary_sensor.h"
#include "esphome/components/number/number.h"
#include "esphome/components/stepper/stepper.h"

#include <Arduino_Modulino.h>

// Thin wrapper around Arduino's own ModulinoMotors class (dual H-bridge motor
// driver). The library is dual-mode: two DC channels (A/B) or one stepper,
// switched at runtime. The hub owns the single instance and the current-sense
// telemetry poll; thin number/stepper entities drive it.

namespace esphome {
namespace modulino_motors {

class ModulinoMotorsComponent : public PollingComponent {
 public:
  void set_current_a_sensor(sensor::Sensor *sensor) { current_a_sensor_ = sensor; }
  void set_current_b_sensor(sensor::Sensor *sensor) { current_b_sensor_ = sensor; }
  void set_busy_binary_sensor(binary_sensor::BinarySensor *bs) { busy_binary_sensor_ = bs; }

  void set_decay(uint8_t decay) {
    decay_ = decay;
    has_decay_ = true;
  }
  void set_pwm_frequency(uint16_t frequency) {
    pwm_frequency_ = frequency;
    has_pwm_frequency_ = true;
  }
  void set_half_step(bool enabled) {
    half_step_ = enabled;
    has_half_step_ = true;
  }
  // Called by the stepper platform so the hub enters stepper mode on setup.
  void set_stepper_mode(bool enabled) { stepper_mode_ = enabled; }

  void setup() override;
  void update() override;
  void dump_config() override;

  // DC: set one channel's signed speed as a percentage (-100..100).
  void set_channel_speed(bool is_b, float percent);
  // Stepper: issue a relative move of `steps` paced at `speed_period`
  // (0.1 ms timer ticks between steps). Returns false if the device is absent.
  bool move_stepper(int32_t steps, uint16_t speed_period);
  bool is_found() const { return found_; }

 protected:
  void apply_settings_();

  ModulinoMotors motors_;
  bool found_{false};
  bool stepper_mode_{false};

  bool has_decay_{false};
  uint8_t decay_{0};
  bool has_pwm_frequency_{false};
  uint16_t pwm_frequency_{20000};
  bool has_half_step_{false};
  bool half_step_{false};

  int16_t raw_a_{0};
  int16_t raw_b_{0};

  sensor::Sensor *current_a_sensor_{nullptr};
  sensor::Sensor *current_b_sensor_{nullptr};
  binary_sensor::BinarySensor *busy_binary_sensor_{nullptr};
};

// DC channel speed as an ESPHome number (-100..100 %).
class ModulinoMotorsNumber : public number::Number, public Parented<ModulinoMotorsComponent> {
 public:
  void set_channel(bool is_b) { is_b_ = is_b; }

 protected:
  void control(float value) override;
  bool is_b_{false};
};

// Stepper driven through ESPHome's native stepper domain. The Modulino executes
// a whole move autonomously, so each time the target changes we issue a single
// moveStepper() for the delta, paced from the configured max speed.
class ModulinoMotorsStepper : public stepper::Stepper,
                              public Component,
                              public Parented<ModulinoMotorsComponent> {
 public:
  void loop() override;
  void dump_config() override;
};

}  // namespace modulino_motors
}  // namespace esphome
