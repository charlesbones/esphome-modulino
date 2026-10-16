#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"

#include <Arduino_Modulino.h>

// Thin wrapper around Arduino's own ModulinoLight class (LTR381RGB ambient
// light + RGB sensor). Recycles the library's update()/getLux()/getIR()/
// getColor() accessors.

namespace esphome {
namespace modulino_light {

class ModulinoLightComponent : public PollingComponent {
 public:
  void set_illuminance_sensor(sensor::Sensor *sensor) { illuminance_sensor_ = sensor; }
  void set_ir_sensor(sensor::Sensor *sensor) { ir_sensor_ = sensor; }
  void set_red_sensor(sensor::Sensor *sensor) { red_sensor_ = sensor; }
  void set_green_sensor(sensor::Sensor *sensor) { green_sensor_ = sensor; }
  void set_blue_sensor(sensor::Sensor *sensor) { blue_sensor_ = sensor; }

  void setup() override;
  void update() override;
  void dump_config() override;

 protected:
  ModulinoLight light_;
  bool found_{false};
  sensor::Sensor *illuminance_sensor_{nullptr};
  sensor::Sensor *ir_sensor_{nullptr};
  sensor::Sensor *red_sensor_{nullptr};
  sensor::Sensor *green_sensor_{nullptr};
  sensor::Sensor *blue_sensor_{nullptr};
};

}  // namespace modulino_light
}  // namespace esphome
