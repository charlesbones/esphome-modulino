#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"

#include <Arduino_Modulino.h>

// Thin wrapper around Arduino's own ModulinoMovement class (LSM6DSOX 6-axis
// IMU). Recycles the library's update()/getX..getYaw() accessors. The gyro
// axes map roll->x, pitch->y, yaw->z.

namespace esphome {
namespace modulino_movement {

class ModulinoMovementComponent : public PollingComponent {
 public:
  void set_accel_x_sensor(sensor::Sensor *sensor) { accel_x_sensor_ = sensor; }
  void set_accel_y_sensor(sensor::Sensor *sensor) { accel_y_sensor_ = sensor; }
  void set_accel_z_sensor(sensor::Sensor *sensor) { accel_z_sensor_ = sensor; }
  void set_gyro_x_sensor(sensor::Sensor *sensor) { gyro_x_sensor_ = sensor; }
  void set_gyro_y_sensor(sensor::Sensor *sensor) { gyro_y_sensor_ = sensor; }
  void set_gyro_z_sensor(sensor::Sensor *sensor) { gyro_z_sensor_ = sensor; }

  void setup() override;
  void update() override;
  void dump_config() override;

 protected:
  ModulinoMovement movement_;
  bool found_{false};
  sensor::Sensor *accel_x_sensor_{nullptr};
  sensor::Sensor *accel_y_sensor_{nullptr};
  sensor::Sensor *accel_z_sensor_{nullptr};
  sensor::Sensor *gyro_x_sensor_{nullptr};
  sensor::Sensor *gyro_y_sensor_{nullptr};
  sensor::Sensor *gyro_z_sensor_{nullptr};
};

}  // namespace modulino_movement
}  // namespace esphome
