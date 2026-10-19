#pragma once

#include "esphome/core/component.h"
#include "esphome/core/helpers.h"
#include "esphome/components/switch/switch.h"
#include "esphome/components/number/number.h"

#include <Arduino_Modulino.h>

// Thin wrapper around Arduino's own ModulinoVibro class, exposed as a switch.
// The switch owns the motor and its intensity (strength) and duration (length).
// With duration = 0 the switch vibrates continuously until turned off (the
// bounded firmware command is re-sent before it elapses); with duration > 0 a
// turn-on is a single pulse of that length, after which the switch returns off.
// The optional number platform drives intensity/duration from Home Assistant.

namespace esphome {
namespace modulino_vibro {

class ModulinoVibroSwitch : public switch_::Switch, public Component {
 public:
  void setup() override;
  void loop() override;
  void dump_config() override;

  void set_intensity(float percent);
  void set_duration(uint32_t duration_ms) {
    duration_ms_ = duration_ms;
    if (active_) on_since_ = millis();  // changing length mid-run restarts the timer, not stops it
  }

 protected:
  void write_state(bool state) override;
  bool ensure_found_();
  int power_() const;

  ModulinoVibro vibro_;
  bool found_{false};
  bool active_{false};
  float intensity_{100.0f};     // strength, 0..100 %
  uint32_t duration_ms_{0};     // length, 0 = continuous while on
  uint32_t last_trigger_{0};
  uint32_t on_since_{0};
};

enum class ModulinoVibroNumberType : uint8_t { INTENSITY, DURATION };

class ModulinoVibroNumber : public number::Number, public Parented<ModulinoVibroSwitch> {
 public:
  void set_type(ModulinoVibroNumberType type) { type_ = type; }

 protected:
  void control(float value) override;
  ModulinoVibroNumberType type_{ModulinoVibroNumberType::INTENSITY};
};

}  // namespace modulino_vibro
}  // namespace esphome
