#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"

#include <Arduino_Modulino.h>

// Thin wrapper around Arduino's own Arduino_Modulino library — recycles its
// ModulinoThermo class (I2C address 0x44, HS300x-family chip) rather than
// re-deriving the sensor's register protocol. setup()/update() mirror the
// exact begin()/getTemperature()/getHumidity() pattern used throughout the
// sibling Arduino-sketch satellite firmware.

namespace esphome {
namespace modulino_thermo {

class ModulinoThermoComponent : public PollingComponent {
 public:
  void set_temperature_sensor(sensor::Sensor *sensor) { temperature_sensor_ = sensor; }
  void set_humidity_sensor(sensor::Sensor *sensor) { humidity_sensor_ = sensor; }

  void setup() override;
  void update() override;
  void dump_config() override;

 protected:
  ModulinoThermo thermo_;
  bool found_{false};
  sensor::Sensor *temperature_sensor_{nullptr};
  sensor::Sensor *humidity_sensor_{nullptr};
};

}  // namespace modulino_thermo
}  // namespace esphome
