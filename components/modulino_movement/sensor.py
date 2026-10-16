import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor
from esphome.const import (
    CONF_ACCELERATION_X,
    CONF_ACCELERATION_Y,
    CONF_ACCELERATION_Z,
    CONF_GYROSCOPE_X,
    CONF_GYROSCOPE_Y,
    CONF_GYROSCOPE_Z,
    CONF_ID,
    STATE_CLASS_MEASUREMENT,
    UNIT_DEGREE_PER_SECOND,
    UNIT_G,
)

from esphome.components.modulino import add_modulino_base

from . import modulino_movement_ns

# Wraps Arduino's ModulinoMovement (LSM6DSOX 6-axis IMU). Mirrors the
# single-domain Thermo component, publishing the three acceleration axes (g)
# and the three gyroscope axes (°/s).

ModulinoMovementComponent = modulino_movement_ns.class_(
    "ModulinoMovementComponent", cg.PollingComponent
)


def _accel_schema():
    return sensor.sensor_schema(
        unit_of_measurement=UNIT_G,
        accuracy_decimals=3,
        state_class=STATE_CLASS_MEASUREMENT,
    )


def _gyro_schema():
    return sensor.sensor_schema(
        unit_of_measurement=UNIT_DEGREE_PER_SECOND,
        accuracy_decimals=1,
        state_class=STATE_CLASS_MEASUREMENT,
    )


CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(ModulinoMovementComponent),
        cv.Optional(CONF_ACCELERATION_X): _accel_schema(),
        cv.Optional(CONF_ACCELERATION_Y): _accel_schema(),
        cv.Optional(CONF_ACCELERATION_Z): _accel_schema(),
        cv.Optional(CONF_GYROSCOPE_X): _gyro_schema(),
        cv.Optional(CONF_GYROSCOPE_Y): _gyro_schema(),
        cv.Optional(CONF_GYROSCOPE_Z): _gyro_schema(),
    }
).extend(cv.polling_component_schema("60s"))


async def to_code(config):
    add_modulino_base()
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)

    for key, setter in (
        (CONF_ACCELERATION_X, "set_accel_x_sensor"),
        (CONF_ACCELERATION_Y, "set_accel_y_sensor"),
        (CONF_ACCELERATION_Z, "set_accel_z_sensor"),
        (CONF_GYROSCOPE_X, "set_gyro_x_sensor"),
        (CONF_GYROSCOPE_Y, "set_gyro_y_sensor"),
        (CONF_GYROSCOPE_Z, "set_gyro_z_sensor"),
    ):
        if key in config:
            sens = await sensor.new_sensor(config[key])
            cg.add(getattr(var, setter)(sens))
