#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"

#include <Arduino_Modulino.h>

// Thin wrapper around Arduino's own ModulinoDistance class (VL53L4CD/VL53L4ED
// time-of-flight sensor). Recycles the library's available()/get() accessors;
// a single distance reading in millimetres, so the component is the sensor.

namespace esphome {
namespace modulino_distance {

class ModulinoDistanceComponent : public sensor::Sensor, public PollingComponent {
 public:
  void setup() override;
  void update() override;
  void dump_config() override;

 protected:
  ModulinoDistance distance_;
  bool found_{false};
};

}  // namespace modulino_distance
}  // namespace esphome
