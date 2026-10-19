import esphome.codegen as cg
from esphome.components import number, switch

# The switch header pulls in the number domain, so a switch-only config (no
# intensity/duration sliders) still compiles.
AUTO_LOAD = ["number"]

modulino_vibro_ns = cg.esphome_ns.namespace("modulino_vibro")

# The switch owns the ModulinoVibro instance and the intensity/duration state;
# the optional number platform attaches intensity (strength) and duration
# (length) controls to it. Defined here so switch.py and number.py share them.
ModulinoVibroSwitch = modulino_vibro_ns.class_(
    "ModulinoVibroSwitch", switch.Switch, cg.Component
)
ModulinoVibroNumber = modulino_vibro_ns.class_(
    "ModulinoVibroNumber", number.Number, cg.Parented.template(ModulinoVibroSwitch)
)
VibroNumberType = modulino_vibro_ns.enum("ModulinoVibroNumberType", is_class=True)

CONF_MODULINO_VIBRO_ID = "modulino_vibro_id"
