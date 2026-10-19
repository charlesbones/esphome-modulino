import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import display
from esphome.const import CONF_ID, CONF_LAMBDA, CONF_PAGES

from esphome.components.modulino import add_modulino_base

from . import modulino_led_matrix_ns

# Wraps Arduino's ModulinoLEDMatrix (12x8 monochrome) as an ESPHome display:
# draw with a lambda like any other display. Scrolling text and animations are
# just content you draw from the lambda (see example-led-matrix.yaml).

ModulinoLedMatrix = modulino_led_matrix_ns.class_(
    "ModulinoLedMatrix", display.DisplayBuffer
)

CONFIG_SCHEMA = cv.All(
    display.FULL_DISPLAY_SCHEMA.extend(
        {
            cv.GenerateID(): cv.declare_id(ModulinoLedMatrix),
        }
    ).extend(cv.polling_component_schema("200ms")),
    cv.has_at_most_one_key(CONF_PAGES, CONF_LAMBDA),
)


async def to_code(config):
    add_modulino_base()
    # The matrix library draws via ArduinoGraphics.
    cg.add_library("ArduinoGraphics", None)
    var = cg.new_Pvariable(config[CONF_ID])
    await display.register_display(var, config)
    if CONF_LAMBDA in config:
        lambda_ = await cg.process_lambda(
            config[CONF_LAMBDA], [(display.DisplayRef, "it")], return_type=cg.void
        )
        cg.add(var.set_writer(lambda_))
