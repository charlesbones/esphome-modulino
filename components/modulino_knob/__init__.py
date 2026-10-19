import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components.modulino import add_modulino_base
from esphome.const import CONF_ID

# Knob exposes two entities of different domains (a rotary position sensor and
# a push-button binary_sensor), so — unlike the single-domain Thermo
# component — it uses ESPHome's hub + platform pattern (mirroring
# esphome/components/as3935): this top-level component owns the single
# ModulinoKnob I2C instance, and the sensor/ and binary_sensor/ platforms
# attach their entities to it. One shared instance matters here because
# ModulinoKnob tracks an internal relative position counter that two
# independent instances would corrupt.

CODEOWNERS = ["@arduino-libraries"]
# The header pulls in both domains, so a position-only or button-only config
# still compiles.
AUTO_LOAD = ["binary_sensor", "sensor"]

modulino_knob_ns = cg.esphome_ns.namespace("modulino_knob")
ModulinoKnobComponent = modulino_knob_ns.class_(
    "ModulinoKnobComponent", cg.PollingComponent
)

CONF_MODULINO_KNOB_ID = "modulino_knob_id"

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(ModulinoKnobComponent),
    }
).extend(cv.polling_component_schema("60s"))


async def to_code(config):
    add_modulino_base()
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
