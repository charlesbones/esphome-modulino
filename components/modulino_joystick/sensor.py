import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor
from esphome.const import CONF_X, CONF_Y, STATE_CLASS_MEASUREMENT

from . import CONF_MODULINO_JOYSTICK_ID, ModulinoJoystickComponent

DEPENDENCIES = ["modulino_joystick"]


def _axis_schema():
    return sensor.sensor_schema(
        accuracy_decimals=0,
        state_class=STATE_CLASS_MEASUREMENT,
    )


CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(CONF_MODULINO_JOYSTICK_ID): cv.use_id(ModulinoJoystickComponent),
        cv.Optional(CONF_X): _axis_schema(),
        cv.Optional(CONF_Y): _axis_schema(),
    }
)


async def to_code(config):
    hub = await cg.get_variable(config[CONF_MODULINO_JOYSTICK_ID])
    if CONF_X in config:
        sens = await sensor.new_sensor(config[CONF_X])
        cg.add(hub.set_x_sensor(sens))
    if CONF_Y in config:
        sens = await sensor.new_sensor(config[CONF_Y])
        cg.add(hub.set_y_sensor(sens))
