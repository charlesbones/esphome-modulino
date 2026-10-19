#pragma once

#include "esphome/core/component.h"
#include "esphome/components/switch/switch.h"

#include <Arduino_Modulino.h>

// Thin wrapper around Arduino's own ModulinoBuzzer class. Exposed as a switch:
// ON plays a continuous tone at the configured frequency, OFF silences it.
// The buzzer firmware only plays a tone for a bounded duration, so while the
// switch is on the tone is periodically re-sent (see modulino_buzzer.cpp).

namespace esphome {
namespace modulino_buzzer {

class ModulinoBuzzerSwitch : public switch_::Switch, public Component {
 public:
  void setup() override;
  void loop() override;
  void dump_config() override;
  void set_frequency(uint32_t frequency) { frequency_ = frequency; }

 protected:
  void write_state(bool state) override;
  bool ensure_found_();

  ModulinoBuzzer buzzer_;
  bool found_{false};
  bool active_{false};
  uint32_t frequency_{440};
  uint32_t last_trigger_{0};
};

}  // namespace modulino_buzzer
}  // namespace esphome
