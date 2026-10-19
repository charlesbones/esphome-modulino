import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import switch

from esphome.components.modulino import add_modulino_base

from . import ModulinoVibroSwitch

# Wraps Arduino's ModulinoVibro as an on/off switch. ON runs the motor (at the
# configured intensity; continuously, or for the configured duration as a
# single pulse); OFF stops it. Intensity and duration are set via the optional
# number platform — give this switch an `id:` to attach them.

CONFIG_SCHEMA = switch.switch_schema(ModulinoVibroSwitch).extend(cv.COMPONENT_SCHEMA)


async def to_code(config):
    add_modulino_base()
    var = await switch.new_switch(config)
    await cg.register_component(var, config)
