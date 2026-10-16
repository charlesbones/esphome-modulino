import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import binary_sensor
from esphome.const import CONF_ID

from esphome.components.modulino import add_modulino_base

from . import modulino_buttons_ns

# Wraps Arduino's ModulinoButtons (three capacitive buttons A/B/C). All three
# entities are binary_sensors, so — like the single-domain Thermo component —
# one polling component owns them directly (no hub needed).

CONF_BUTTON_A = "button_a"
CONF_BUTTON_B = "button_b"
CONF_BUTTON_C = "button_c"

ModulinoButtonsComponent = modulino_buttons_ns.class_(
    "ModulinoButtonsComponent", cg.PollingComponent
)

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(ModulinoButtonsComponent),
        cv.Optional(CONF_BUTTON_A): binary_sensor.binary_sensor_schema(),
        cv.Optional(CONF_BUTTON_B): binary_sensor.binary_sensor_schema(),
        cv.Optional(CONF_BUTTON_C): binary_sensor.binary_sensor_schema(),
    }
).extend(cv.polling_component_schema("60s"))


async def to_code(config):
    add_modulino_base()
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)

    for key, setter in (
        (CONF_BUTTON_A, "set_button_a_binary_sensor"),
        (CONF_BUTTON_B, "set_button_b_binary_sensor"),
        (CONF_BUTTON_C, "set_button_c_binary_sensor"),
    ):
        if key in config:
            bin_sens = await binary_sensor.new_binary_sensor(config[key])
            cg.add(getattr(var, setter)(bin_sens))
