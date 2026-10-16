import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import binary_sensor
from esphome.const import DEVICE_CLASS_RUNNING

from . import CONF_MODULINO_MOTORS_ID, ModulinoMotorsComponent

DEPENDENCIES = ["modulino_motors"]

# A single "busy" binary_sensor reporting whether a move is in progress.
CONFIG_SCHEMA = binary_sensor.binary_sensor_schema(
    device_class=DEVICE_CLASS_RUNNING
).extend(
    {
        cv.GenerateID(CONF_MODULINO_MOTORS_ID): cv.use_id(ModulinoMotorsComponent),
    }
)


async def to_code(config):
    hub = await cg.get_variable(config[CONF_MODULINO_MOTORS_ID])
    bin_sens = await binary_sensor.new_binary_sensor(config)
    cg.add(hub.set_busy_binary_sensor(bin_sens))
