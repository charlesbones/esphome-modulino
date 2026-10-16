import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor
from esphome.const import (
    CONF_CHANNEL,
    DEVICE_CLASS_CURRENT,
    STATE_CLASS_MEASUREMENT,
    UNIT_MILLIAMP,
)

from . import CONF_MODULINO_MOTORS_ID, ModulinoMotorsComponent

DEPENDENCIES = ["modulino_motors"]

CHANNELS = ("a", "b")

CONFIG_SCHEMA = sensor.sensor_schema(
    unit_of_measurement=UNIT_MILLIAMP,
    accuracy_decimals=0,
    device_class=DEVICE_CLASS_CURRENT,
    state_class=STATE_CLASS_MEASUREMENT,
).extend(
    {
        cv.GenerateID(CONF_MODULINO_MOTORS_ID): cv.use_id(ModulinoMotorsComponent),
        cv.Required(CONF_CHANNEL): cv.one_of(*CHANNELS, lower=True),
    }
)


async def to_code(config):
    hub = await cg.get_variable(config[CONF_MODULINO_MOTORS_ID])
    sens = await sensor.new_sensor(config)
    if config[CONF_CHANNEL] == "a":
        cg.add(hub.set_current_a_sensor(sens))
    else:
        cg.add(hub.set_current_b_sensor(sens))
