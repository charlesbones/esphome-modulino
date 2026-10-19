import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.const import CONF_ID

from esphome.components.modulino import add_modulino_base

# ModulinoMotors is a dual-mode controller (two DC channels OR one stepper,
# mutually exclusive). This hub owns the single ModulinoMotors I2C instance and
# the current telemetry poll; the number/ platform drives DC speeds, the sensor/
# and binary_sensor/ platforms publish current + busy telemetry, and the stepper/
# platform drives it as an ESPHome stepper. The hub switches to stepper mode
# automatically when a stepper platform is attached.

CODEOWNERS = ["@arduino-libraries"]
# The component's header pulls in all of these domains, so load them even when a
# given config only uses some of the platforms (e.g. DC-only, no stepper).
AUTO_LOAD = ["binary_sensor", "number", "sensor", "stepper"]

modulino_motors_ns = cg.esphome_ns.namespace("modulino_motors")
ModulinoMotorsComponent = modulino_motors_ns.class_(
    "ModulinoMotorsComponent", cg.PollingComponent
)

CONF_MODULINO_MOTORS_ID = "modulino_motors_id"

CONF_DECAY = "decay"
CONF_PWM_FREQUENCY = "pwm_frequency"
CONF_HALF_STEP = "half_step"

# Maps to ModulinoMotors::DecayMode (0..3).
DECAY_MODES = {
    "slow": 0,
    "mixed_30_fast_70_slow": 1,
    "mixed_60_fast_40_slow": 2,
    "fast": 3,
}

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(ModulinoMotorsComponent),
        cv.Optional(CONF_DECAY): cv.enum(DECAY_MODES, lower=True),
        cv.Optional(CONF_PWM_FREQUENCY): cv.int_range(min=200, max=60000),
        cv.Optional(CONF_HALF_STEP): cv.boolean,
    }
).extend(cv.polling_component_schema("60s"))


async def to_code(config):
    add_modulino_base()
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    if CONF_DECAY in config:
        cg.add(var.set_decay(config[CONF_DECAY]))
    if CONF_PWM_FREQUENCY in config:
        cg.add(var.set_pwm_frequency(config[CONF_PWM_FREQUENCY]))
    if CONF_HALF_STEP in config:
        cg.add(var.set_half_step(config[CONF_HALF_STEP]))
