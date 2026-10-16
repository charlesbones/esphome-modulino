import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import light
from esphome.const import CONF_OUTPUT_ID

from esphome.components.modulino import add_modulino_base

from . import modulino_pixels_ns

ModulinoPixelsLightOutput = modulino_pixels_ns.class_(
    "ModulinoPixelsLightOutput", light.AddressableLight
)

CONFIG_SCHEMA = light.ADDRESSABLE_LIGHT_SCHEMA.extend(
    {
        cv.GenerateID(CONF_OUTPUT_ID): cv.declare_id(ModulinoPixelsLightOutput),
    }
)


async def to_code(config):
    add_modulino_base()
    var = cg.new_Pvariable(config[CONF_OUTPUT_ID])
    await light.register_light(var, config)
    await cg.register_component(var, config)
