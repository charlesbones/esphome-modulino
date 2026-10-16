import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import number
from esphome.const import CONF_CHANNEL

from . import CONF_MODULINO_MOTORS_ID, ModulinoMotorsComponent, modulino_motors_ns

DEPENDENCIES = ["modulino_motors"]

ModulinoMotorsNumber = modulino_motors_ns.class_(
    "ModulinoMotorsNumber", number.Number, cg.Parented.template(ModulinoMotorsComponent)
)

# Maps channel letter -> is_b flag passed to the C++ setter.
CHANNELS = {"a": False, "b": True}

CONFIG_SCHEMA = number.number_schema(ModulinoMotorsNumber).extend(
    {
        cv.GenerateID(CONF_MODULINO_MOTORS_ID): cv.use_id(ModulinoMotorsComponent),
        cv.Required(CONF_CHANNEL): cv.enum(CHANNELS, lower=True),
    }
)


async def to_code(config):
    var = await number.new_number(config, min_value=-100, max_value=100, step=1)
    await cg.register_parented(var, config[CONF_MODULINO_MOTORS_ID])
    cg.add(var.set_channel(config[CONF_CHANNEL]))
