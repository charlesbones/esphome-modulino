import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor
from esphome.const import (
    CONF_BLUE,
    CONF_GREEN,
    CONF_ID,
    CONF_ILLUMINANCE,
    CONF_RED,
    DEVICE_CLASS_ILLUMINANCE,
    STATE_CLASS_MEASUREMENT,
    UNIT_LUX,
)

from esphome.components.modulino import add_modulino_base

from . import modulino_light_ns

# Wraps Arduino's ModulinoLight (LTR381RGB ambient-light + RGB sensor). Mirrors
# the single-domain Thermo component.

CONF_IR = "ir"

ModulinoLightComponent = modulino_light_ns.class_(
    "ModulinoLightComponent", cg.PollingComponent
)


def _channel_schema():
    return sensor.sensor_schema(
        accuracy_decimals=0,
        state_class=STATE_CLASS_MEASUREMENT,
    )


CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(ModulinoLightComponent),
        cv.Optional(CONF_ILLUMINANCE): sensor.sensor_schema(
            unit_of_measurement=UNIT_LUX,
            accuracy_decimals=0,
            device_class=DEVICE_CLASS_ILLUMINANCE,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_IR): _channel_schema(),
        cv.Optional(CONF_RED): _channel_schema(),
        cv.Optional(CONF_GREEN): _channel_schema(),
        cv.Optional(CONF_BLUE): _channel_schema(),
    }
).extend(cv.polling_component_schema("60s"))


async def to_code(config):
    add_modulino_base()
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)

    for key, setter in (
        (CONF_ILLUMINANCE, "set_illuminance_sensor"),
        (CONF_IR, "set_ir_sensor"),
        (CONF_RED, "set_red_sensor"),
        (CONF_GREEN, "set_green_sensor"),
        (CONF_BLUE, "set_blue_sensor"),
    ):
        if key in config:
            sens = await sensor.new_sensor(config[key])
            cg.add(getattr(var, setter)(sens))
