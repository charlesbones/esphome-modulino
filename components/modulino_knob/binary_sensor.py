import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import binary_sensor

from . import CONF_MODULINO_KNOB_ID, ModulinoKnobComponent

DEPENDENCIES = ["modulino_knob"]

CONFIG_SCHEMA = binary_sensor.binary_sensor_schema().extend(
    {
        cv.GenerateID(CONF_MODULINO_KNOB_ID): cv.use_id(ModulinoKnobComponent),
    }
)


async def to_code(config):
    hub = await cg.get_variable(config[CONF_MODULINO_KNOB_ID])
    bin_sens = await binary_sensor.new_binary_sensor(config)
    cg.add(hub.set_pressed_binary_sensor(bin_sens))
