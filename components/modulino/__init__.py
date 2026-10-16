"""Shared base for all modulino_* components.

Centralizes everything needed to compile against Arduino's own
``Arduino_Modulino`` library so each wrapper component (modulino_knob,
modulino_pixels, ...) only has to ``DEPENDENCIES = ["modulino"]`` and call
``add_modulino_base()`` from its ``to_code`` — nothing about the dependency
graph is duplicated per component.

Two things make Arduino_Modulino build under ESPHome's ESP-IDF + Arduino
toolchain:

1. ``Arduino_Modulino`` is vendored here with a ``library.json`` that declares
   its full dependency graph. ESPHome only resolves dependencies from the JSON
   ``dependencies`` key, never ``library.properties`` ``depends=`` (which is all
   the upstream library ships), so without this the library's own ``Modulino.cpp``
   cannot find its sibling libraries' headers (e.g. ``vl53l4cd_class.h``).
2. The umbrella header pulls in ``Arduino_LSM6DSOX``, which ``#include``s
   ``<SPI.h>``. ESPHome exposes the Arduino core's bundled ``Wire`` library but
   not ``SPI``; ``CONFIG_ARDUINO_SELECTIVE_SPI`` adds it.
"""

import os

import esphome.codegen as cg

modulino_ns = cg.esphome_ns.namespace("modulino")

_ARDUINO_MODULINO_DIR = os.path.join(os.path.dirname(__file__), "Arduino_Modulino")

# Bundled Arduino-core libraries the Modulino libraries include: Wire (I2C, used
# by every node) and SPI (included by Arduino_LSM6DSOX's header). Under ESPHome's
# ESP-IDF + Arduino build these ship with the core but are compiled only when
# selected; cg.add_library() with the exact Arduino library name is what selects
# them (CONFIG_ARDUINO_SELECTIVE_*), and ESPHome knows they are core-provided so
# it does not try to fetch them from the PlatformIO registry.
_ARDUINO_CORE_LIBRARIES = (
    "Wire",
    "SPI",
)

# Arduino_Modulino's sibling dependency libraries. They are added as top-level
# libraries (not only as Arduino_Modulino's transitive dependencies) so each is
# wired to the Arduino core component and resolves Wire/SPI; the same set is
# also declared in the vendored library.json so Arduino_Modulino's own component
# REQUIRES them and finds their headers.
_MODULINO_DEP_LIBRARIES = (
    "STM32duino VL53L4CD",
    "STM32duino VL53L4ED",
    "Arduino_LSM6DSOX",
    "Arduino_LPS22HB",
    "Arduino_HS300x",
    "Arduino_LTR381RGB",
    "SHT31",
)

# Default I2C pins for the Modulino/Qwiic bus. ESPHome's generic esp32s3 Arduino
# variant defaults the Arduino `Wire` object to GPIO8/9, but the Arduino Nano
# ESP32's Qwiic connector — like most Qwiic boards — is on A4/A5 = GPIO11/GPIO12.
# Components call Wire.begin(MODULINO_I2C_SDA, MODULINO_I2C_SCL) before
# Modulino.begin() so the library's bus comes up on the right pins. Override per
# config with esphome -> platformio_options -> build_flags, e.g.
#   -DMODULINO_I2C_SDA=8 -DMODULINO_I2C_SCL=9
DEFAULT_SDA_PIN = 11
DEFAULT_SCL_PIN = 12


def add_modulino_base():
    """Wire the Arduino_Modulino library (and its full dependency tree) into
    the build. Safe to call from every component — ESPHome de-duplicates
    libraries and build flags."""
    for _lib in (*_ARDUINO_CORE_LIBRARIES, *_MODULINO_DEP_LIBRARIES):
        cg.add_library(_lib, None)
    cg.add_library("Arduino_Modulino", None, f"file://{_ARDUINO_MODULINO_DIR}")
    cg.add_build_flag(f"-DMODULINO_I2C_SDA={DEFAULT_SDA_PIN}")
    cg.add_build_flag(f"-DMODULINO_I2C_SCL={DEFAULT_SCL_PIN}")
