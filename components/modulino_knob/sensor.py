import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor
from esphome.const import STATE_CLASS_MEASUREMENT

from . import CONF_MODULINO_KNOB_ID, ModulinoKnobComponent

DEPENDENCIES = ["modulino_knob"]

CONFIG_SCHEMA = sensor.sensor_schema(
    accuracy_decimals=0,
    state_class=STATE_CLASS_MEASUREMENT,
).extend(
    {
        cv.GenerateID(CONF_MODULINO_KNOB_ID): cv.use_id(ModulinoKnobComponent),
    }
)


async def to_code(config):
    hub = await cg.get_variable(config[CONF_MODULINO_KNOB_ID])
    sens = await sensor.new_sensor(config)
    cg.add(hub.set_position_sensor(sens))
