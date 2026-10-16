#pragma once

#include "esphome/core/component.h"
#include "esphome/components/switch/switch.h"

#include <Arduino_Modulino.h>

// Thin wrapper around Arduino's own ModulinoVibro class. Exposed as a switch:
// ON drives the vibration motor continuously, OFF stops it.

namespace esphome {
namespace modulino_vibro {

class ModulinoVibroSwitch : public switch_::Switch, public Component {
 public:
  void setup() override;
  void dump_config() override;

 protected:
  void write_state(bool state) override;

  ModulinoVibro vibro_;
  bool found_{false};
};

}  // namespace modulino_vibro
}  // namespace esphome
