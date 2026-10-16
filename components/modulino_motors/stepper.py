import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import stepper
from esphome.const import CONF_ID

from . import CONF_MODULINO_MOTORS_ID, ModulinoMotorsComponent, modulino_motors_ns

DEPENDENCIES = ["modulino_motors"]

ModulinoMotorsStepper = modulino_motors_ns.class_(
    "ModulinoMotorsStepper",
    stepper.Stepper,
    cg.Component,
    cg.Parented.template(ModulinoMotorsComponent),
)

CONFIG_SCHEMA = stepper.STEPPER_SCHEMA.extend(
    {
        cv.Required(CONF_ID): cv.declare_id(ModulinoMotorsStepper),
        cv.GenerateID(CONF_MODULINO_MOTORS_ID): cv.use_id(ModulinoMotorsComponent),
    }
).extend(cv.COMPONENT_SCHEMA)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await stepper.register_stepper(var, config)
    await cg.register_parented(var, config[CONF_MODULINO_MOTORS_ID])
    # Attaching a stepper puts the shared hub into stepper mode on setup.
    hub = await cg.get_variable(config[CONF_MODULINO_MOTORS_ID])
    cg.add(hub.set_stepper_mode(True))
