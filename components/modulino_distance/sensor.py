import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor
from esphome.const import (
    DEVICE_CLASS_DISTANCE,
    STATE_CLASS_MEASUREMENT,
    UNIT_MILLIMETER,
)

from esphome.components.modulino import add_modulino_base

from . import modulino_distance_ns

# Wraps Arduino's ModulinoDistance (VL53L4CD/VL53L4ED time-of-flight sensor).
# A single distance reading in mm, so the component is itself the sensor.

ModulinoDistanceComponent = modulino_distance_ns.class_(
    "ModulinoDistanceComponent", sensor.Sensor, cg.PollingComponent
)

CONFIG_SCHEMA = sensor.sensor_schema(
    ModulinoDistanceComponent,
    unit_of_measurement=UNIT_MILLIMETER,
    accuracy_decimals=0,
    device_class=DEVICE_CLASS_DISTANCE,
    state_class=STATE_CLASS_MEASUREMENT,
).extend(cv.polling_component_schema("60s"))


async def to_code(config):
    add_modulino_base()
    var = await sensor.new_sensor(config)
    await cg.register_component(var, config)
