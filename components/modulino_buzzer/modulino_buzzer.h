#pragma once

#include "esphome/core/component.h"
#include "esphome/components/switch/switch.h"

#include <Arduino_Modulino.h>

// Thin wrapper around Arduino's own ModulinoBuzzer class. Exposed as a switch:
// ON plays a continuous tone at the configured frequency, OFF silences it.

namespace esphome {
namespace modulino_buzzer {

class ModulinoBuzzerSwitch : public switch_::Switch, public Component {
 public:
  void setup() override;
  void dump_config() override;
  void set_frequency(uint32_t frequency) { frequency_ = frequency; }

 protected:
  void write_state(bool state) override;

  ModulinoBuzzer buzzer_;
  bool found_{false};
  uint32_t frequency_{440};
};

}  // namespace modulino_buzzer
}  // namespace esphome
