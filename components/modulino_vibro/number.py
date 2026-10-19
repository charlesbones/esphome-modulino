import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import number
from esphome.const import CONF_TYPE

from . import (
    CONF_MODULINO_VIBRO_ID,
    ModulinoVibroNumber,
    ModulinoVibroSwitch,
    VibroNumberType,
)

# Attaches intensity (strength) and/or duration (length) controls to a
# modulino_vibro switch. Reference the switch's id via modulino_vibro_id.
#   type: intensity -> 0..100 (% strength)
#   type: duration  -> 0..10000 (ms; 0 = vibrate continuously while on)
TYPE_PARAMS = {
    "intensity": (VibroNumberType.INTENSITY, 0, 100, 1),
    "duration": (VibroNumberType.DURATION, 0, 10000, 10),
}

CONFIG_SCHEMA = number.number_schema(ModulinoVibroNumber).extend(
    {
        cv.GenerateID(CONF_MODULINO_VIBRO_ID): cv.use_id(ModulinoVibroSwitch),
        cv.Required(CONF_TYPE): cv.one_of(*TYPE_PARAMS, lower=True),
    }
)


async def to_code(config):
    enum_val, lo, hi, step = TYPE_PARAMS[config[CONF_TYPE]]
    var = await number.new_number(config, min_value=lo, max_value=hi, step=step)
    await cg.register_parented(var, config[CONF_MODULINO_VIBRO_ID])
    cg.add(var.set_type(enum_val))
