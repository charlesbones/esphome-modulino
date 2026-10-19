# esphome-modulino

ESPHome external components for the [Arduino Modulino](https://www.arduino.cc/education/modulino-nodes/)
sensor/actuator nodes. Each component is a thin wrapper around Arduino's own
[`Arduino_Modulino`](https://github.com/arduino-libraries/Arduino_Modulino)
library — none of the I2C protocols are reimplemented — so the nodes drop
straight into an ESPHome project and show up in Home Assistant.

Targets the ESP32-S3 boards the Modulino ecosystem uses (Arduino Nano ESP32,
UNO R4, etc.) under the `arduino` framework.

## Quick start

1. Install ESPHome (`pipx install esphome` or `pip install esphome`).
2. Copy `secrets.yaml.example` to `secrets.yaml` and fill in your Wi-Fi.
3. Edit one of the example configs to list the nodes you have, then:

```bash
esphome run example-full.yaml
```

The example configs:

- `example-thermo.yaml` — minimal, one Thermo node (good starting point).
- `example-full.yaml` — every readable/controllable node wired up at once.
- `example-motors.yaml` — the dual-mode Motors node (DC or stepper).
- `example-pixels.yaml` — per-LED control of the Pixels strip (8 individual
  Home Assistant lights via `partition`, plus addressable effects).
- `example-vibro.yaml` — Vibro with intensity (strength) and duration (length)
  number controls.
- `example-led-matrix.yaml` — LED Matrix as a display: draw, scroll text from a
  Home Assistant text box, or play an animation picked from a select.

## Use it in your own project

Point `external_components` at this repo instead of copying files:

```yaml
external_components:
  - source: github://YOUR_GITHUB_USERNAME/esphome-modulino@main
```

Or vendor it locally (what the examples do):

```yaml
external_components:
  - source:
      type: local
      path: components
```

Then add the nodes you have — for example:

```yaml
sensor:
  - platform: modulino_thermo
    temperature:
      name: "Temperature"
    humidity:
      name: "Humidity"
```

## Components

- `modulino_thermo` — `sensor`: temperature, humidity (HS300x/SHT31).
- `modulino_light` — `sensor`: illuminance (lx), IR, red/green/blue (LTR381RGB).
- `modulino_movement` — `sensor`: 3× acceleration (g), 3× gyroscope (°/s) (LSM6DSOX).
- `modulino_distance` — `sensor`: distance (mm) (VL53L4CD/VL53L4ED).
- `modulino_knob` — `sensor` (position) + `binary_sensor` (press) (rotary encoder).
- `modulino_joystick` — `sensor` (x/y) + `binary_sensor` (press).
- `modulino_buttons` — `binary_sensor` ×3 (buttons A/B/C).
- `modulino_pixels` — `light`: 8-LED addressable RGB strip.
- `modulino_buzzer` — `switch`: continuous tone at a configurable frequency.
- `modulino_vibro` — `switch`: vibration motor on/off, with optional `number`
  controls for intensity (strength, %) and duration (length, ms; 0 = continuous).
- `modulino_latch_relay` — `switch`: latching relay set/reset.
- `modulino_motors` — dual-mode H-bridge: two DC channels (`number` speed +
  `sensor` current + `binary_sensor` busy) **or** one stepper (native ESPHome
  `stepper` domain). The two modes are mutually exclusive.
- `modulino_led_matrix` — `display`: 12x8 monochrome matrix. Draw with a display
  lambda; scroll text or play animations by combining it with stock ESPHome
  `text`/`select` inputs (see `example-led-matrix.yaml`).

Not implemented: `modulino_opto_relay`.

### Refresh rate

Every readable component polls at `update_interval` (default **60s**). Lower it
for interactive inputs (knob / joystick / buttons) so presses feel responsive —
see `example-full.yaml`, which uses `250ms`. `update_interval` goes on the
platform entry, or on the hub block for knob / joystick / motors. The `light`
and `switch` components act on command and don't poll.

### I2C pins

The Modulino nodes use the Arduino `Wire` bus (not ESPHome's `i2c:` component —
on ESP32 that is a separate ESP-IDF bus). ESPHome's generic `esp32s3` Arduino
variant defaults `Wire` to **GPIO8/GPIO9**, but the Arduino Nano ESP32's Qwiic
connector (A4/A5) is on **GPIO11/GPIO12**, so each component brings `Wire` up on
GPIO11/GPIO12 before talking to the library. If your wiring differs, override it
with build flags — no code change needed:

```yaml
esphome:
  platformio_options:
    build_flags:
      - -DMODULINO_I2C_SDA=8
      - -DMODULINO_I2C_SCL=9
```

Do **not** add an ESPHome `i2c:` block for the Modulinos — it configures the
unrelated IDF bus, not the `Wire` pins the library uses.

## How it works (dependency wiring)

ESPHome 2026.x builds the Arduino framework on top of ESP-IDF (CMake), where
every Arduino library becomes an isolated ESP-IDF component. Two things that
"just work" under the old PlatformIO Arduino builder no longer do, so the shared
[`components/modulino`](components/modulino) base component fixes them once for
every wrapper:

1. **Inter-library includes.** `Arduino_Modulino`'s umbrella header pulls in its
   sibling libraries (`vl53l4cd_class.h`, `Arduino_LSM6DSOX.h`, …). ESPHome only
   resolves a library's dependencies from the JSON `dependencies` key, never
   from `library.properties` `depends=` (which is all upstream ships). So the
   library is **vendored** in
   [`components/modulino/Arduino_Modulino`](components/modulino/Arduino_Modulino)
   with an added `library.json` declaring the full, versioned dependency graph.
2. **Bundled Wire/SPI.** The Arduino core ships `Wire` and `SPI` but compiles
   them only when selected; `cg.add_library("Wire")` / `cg.add_library("SPI")`
   selects them.

Each wrapper component calls `add_modulino_base()` in its `to_code()` and follows
a standard ESPHome platform pattern (single polling component, hub + platforms,
`AddressableLight`, or `switch`).

The vendored copy is kept **unmodified** from upstream so it stays easy to update:
to pull a new `Arduino_Modulino` release, replace
`components/modulino/Arduino_Modulino/src/` (and `library.properties`) with the
new version and keep our added `library.json`. No source patches to re-apply —
the wrapper components work around upstream quirks in their own code.

## License

This repository is licensed under **GPL-2.0** (see `LICENSE`).

The vendored `components/modulino/Arduino_Modulino/` directory is Arduino's
`Arduino_Modulino` library, redistributed unmodified under its own
**MPL-2.0** license (see the SPDX headers and `library.properties` there).
