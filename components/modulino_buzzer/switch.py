import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import switch
from esphome.const import CONF_FREQUENCY

from esphome.components.modulino import add_modulino_base

from . import modulino_buzzer_ns

# Wraps Arduino's ModulinoBuzzer as an on/off switch: turning it on plays a
# continuous tone at the configured frequency, turning it off silences it.

ModulinoBuzzerSwitch = modulino_buzzer_ns.class_(
    "ModulinoBuzzerSwitch", switch.Switch, cg.Component
)

CONFIG_SCHEMA = switch.switch_schema(ModulinoBuzzerSwitch).extend(
    {
        cv.Optional(CONF_FREQUENCY, default=440): cv.int_range(min=1, max=20000),
    }
).extend(cv.COMPONENT_SCHEMA)


async def to_code(config):
    add_modulino_base()
    var = await switch.new_switch(config)
    await cg.register_component(var, config)
    cg.add(var.set_frequency(config[CONF_FREQUENCY]))
