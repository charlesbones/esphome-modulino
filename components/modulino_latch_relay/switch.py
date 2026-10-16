import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import switch

from esphome.components.modulino import add_modulino_base

from . import modulino_latch_relay_ns

# Wraps Arduino's ModulinoLatchRelay as an on/off switch: ON latches the relay
# closed (set), OFF latches it open (reset). A latching relay holds its state
# without power, which maps cleanly onto a switch.

ModulinoLatchRelaySwitch = modulino_latch_relay_ns.class_(
    "ModulinoLatchRelaySwitch", switch.Switch, cg.Component
)

CONFIG_SCHEMA = switch.switch_schema(ModulinoLatchRelaySwitch).extend(cv.COMPONENT_SCHEMA)


async def to_code(config):
    add_modulino_base()
    var = await switch.new_switch(config)
    await cg.register_component(var, config)
