#pragma once

#include "esphome/core/component.h"
#include "esphome/components/switch/switch.h"

#include <Arduino_Modulino.h>

// Thin wrapper around Arduino's own ModulinoLatchRelay class. Exposed as a
// switch: ON calls set() (relay closed), OFF calls reset() (relay open).

namespace esphome {
namespace modulino_latch_relay {

class ModulinoLatchRelaySwitch : public switch_::Switch, public Component {
 public:
  void setup() override;
  void dump_config() override;

 protected:
  void write_state(bool state) override;

  ModulinoLatchRelay relay_;
  bool found_{false};
};

}  // namespace modulino_latch_relay
}  // namespace esphome
