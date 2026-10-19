import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components.modulino import add_modulino_base
from esphome.const import CONF_ID

# Joystick exposes two X/Y position sensors and a push-button binary_sensor, so
# it uses the hub + platform pattern (like modulino_knob): this component owns
# the single ModulinoJoystick I2C instance and the sensor/binary_sensor
# platforms attach to it, so the X/Y/press state is read from one shared poll.

CODEOWNERS = ["@arduino-libraries"]
# The header pulls in both domains, so an axes-only or button-only config
# still compiles.
AUTO_LOAD = ["binary_sensor", "sensor"]

modulino_joystick_ns = cg.esphome_ns.namespace("modulino_joystick")
ModulinoJoystickComponent = modulino_joystick_ns.class_(
    "ModulinoJoystickComponent", cg.PollingComponent
)

CONF_MODULINO_JOYSTICK_ID = "modulino_joystick_id"

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(ModulinoJoystickComponent),
    }
).extend(cv.polling_component_schema("60s"))


async def to_code(config):
    add_modulino_base()
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
