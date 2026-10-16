import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import switch

from esphome.components.modulino import add_modulino_base

from . import modulino_vibro_ns

# Wraps Arduino's ModulinoVibro as an on/off switch: ON runs the vibration
# motor continuously, OFF stops it.

ModulinoVibroSwitch = modulino_vibro_ns.class_(
    "ModulinoVibroSwitch", switch.Switch, cg.Component
)

CONFIG_SCHEMA = switch.switch_schema(ModulinoVibroSwitch).extend(cv.COMPONENT_SCHEMA)


async def to_code(config):
    add_modulino_base()
    var = await switch.new_switch(config)
    await cg.register_component(var, config)
