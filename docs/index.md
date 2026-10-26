# esphome-modulino — component reference

ESPHome external components for the [Arduino Modulino](https://store.arduino.cc/collections/modulino)
nodes. Each wraps Arduino's own `Arduino_Modulino` library. See the
[repository README](https://github.com/charlesbones/esphome-modulino) for the
project overview and how it works.

## Using the components

Pull them in with `external_components`, then add the nodes you have:

```yaml
external_components:
  - source: github://charlesbones/esphome-modulino@main
```

Common notes that apply to every component:

- **I2C pins.** The nodes use the Arduino `Wire` bus on **GPIO11/GPIO12** by
  default (the Arduino Nano ESP32 Qwiic pins). Override per config with
  `esphome: platformio_options: build_flags: ["-DMODULINO_I2C_SDA=8", "-DMODULINO_I2C_SCL=9"]`.
  Do **not** add an ESPHome `i2c:` block — on ESP32 that is a separate bus.
- **Refresh rate.** Every readable component polls at `update_interval`
  (default `60s`). Put it on the platform entry, or on the hub block for
  knob / joystick / motors. Lower it for interactive inputs.

---

## Sensors

### modulino_thermo
Temperature + humidity (HS300x at 0x44, or SHT31 at 0x45 — auto-detected).

```yaml
sensor:
  - platform: modulino_thermo
    temperature:
      name: "Temperature"
    humidity:
      name: "Humidity"
```

### modulino_light
Ambient light + colour (LTR381RGB). All sub-sensors optional.

```yaml
sensor:
  - platform: modulino_light
    illuminance: { name: "Illuminance" }   # lux
    ir:          { name: "IR" }
    red:         { name: "Red" }
    green:       { name: "Green" }
    blue:        { name: "Blue" }
```

### modulino_movement
6-axis IMU (LSM6DSOX): acceleration (g) + gyroscope (°/s). All optional.

```yaml
sensor:
  - platform: modulino_movement
    acceleration_x: { name: "Accel X" }
    acceleration_y: { name: "Accel Y" }
    acceleration_z: { name: "Accel Z" }
    gyroscope_x:    { name: "Gyro X" }
    gyroscope_y:    { name: "Gyro Y" }
    gyroscope_z:    { name: "Gyro Z" }
```

### modulino_distance
Time-of-flight distance in mm (VL53L4CD/ED). Publishes `unknown` when nothing
is in range.

```yaml
sensor:
  - platform: modulino_distance
    name: "Distance"
    update_interval: 100ms
```

---

## Inputs (hub + platforms)

Knob and Joystick own a shared I2C instance, so the hub block carries the
`id` and `update_interval`; the sensor/binary_sensor platforms reference it.

### modulino_knob
Rotary encoder: position (sensor) + button (binary_sensor).

```yaml
modulino_knob:
  id: knob1
  update_interval: 250ms

sensor:
  - platform: modulino_knob
    modulino_knob_id: knob1
    name: "Knob Position"

binary_sensor:
  - platform: modulino_knob
    modulino_knob_id: knob1
    name: "Knob Button"
```

### modulino_joystick
Analog joystick: X/Y (sensor) + button (binary_sensor).

```yaml
modulino_joystick:
  id: joystick1
  update_interval: 250ms

sensor:
  - platform: modulino_joystick
    modulino_joystick_id: joystick1
    x: { name: "Joystick X" }
    y: { name: "Joystick Y" }

binary_sensor:
  - platform: modulino_joystick
    modulino_joystick_id: joystick1
    name: "Joystick Button"
```

### modulino_buttons
Three capacitive buttons (A/B/C) as binary_sensors. All optional.

```yaml
binary_sensor:
  - platform: modulino_buttons
    update_interval: 250ms
    button_a: { name: "Button A" }
    button_b: { name: "Button B" }
    button_c: { name: "Button C" }
```

---

## Lights

### modulino_pixels
8-LED addressable RGB strip as an ESPHome `light`. For per-LED control in Home
Assistant, add `partition` lights over it.

```yaml
light:
  - platform: modulino_pixels
    id: pixels_light
    name: "Modulino Pixels"
    effects:
      - addressable_rainbow:
  - platform: partition      # one HA light per LED
    name: "Pixel 0"
    segments: [{ id: pixels_light, from: 0, to: 0 }]
  # ...repeat for pixels 1–7
```

### modulino_led_matrix
12×8 monochrome matrix as an ESPHome `display`. Draw with a lambda; scroll text
or play animations by combining it with stock `text`/`select` inputs (see
`example-led-matrix.yaml`).

```yaml
display:
  - platform: modulino_led_matrix
    id: matrix
    update_interval: 120ms
    lambda: |-
      it.rectangle(0, 0, 12, 8);
```

---

## Actuators

### modulino_buzzer
Buzzer as a `switch` — ON plays a continuous tone at `frequency`, OFF stops it.

```yaml
switch:
  - platform: modulino_buzzer
    name: "Buzzer"
    frequency: 440   # Hz
```

### modulino_vibro
Vibration motor as a `switch`, with optional `number` controls for strength and
length. Give the switch an `id` to attach them.

```yaml
switch:
  - platform: modulino_vibro
    id: vibro1
    name: "Vibro"

number:
  - platform: modulino_vibro
    modulino_vibro_id: vibro1
    type: intensity          # 0..100 %
    name: "Vibro Intensity"
  - platform: modulino_vibro
    modulino_vibro_id: vibro1
    type: duration           # ms; 0 = continuous while on, >0 = single pulse
    name: "Vibro Duration"
```

### modulino_latch_relay
Latching relay as a `switch` — ON = set, OFF = reset.

```yaml
switch:
  - platform: modulino_latch_relay
    name: "Latch Relay"
```

### modulino_motors
Dual-mode H-bridge hub. **DC** (two channels) **or** stepper — mutually
exclusive. Hub options: `decay` (slow / mixed_30_fast_70_slow /
mixed_60_fast_40_slow / fast), `pwm_frequency` (200–60000 Hz), `half_step`.

```yaml
modulino_motors:
  id: motors1

# DC mode: speed (-100..100 %) + current (mA) + busy
number:
  - platform: modulino_motors
    modulino_motors_id: motors1
    channel: a
    name: "Motor A Speed"
sensor:
  - platform: modulino_motors
    modulino_motors_id: motors1
    channel: a
    name: "Motor A Current"
binary_sensor:
  - platform: modulino_motors
    modulino_motors_id: motors1
    name: "Motors Busy"

# Stepper mode (instead of the DC blocks): native ESPHome stepper
stepper:
  - platform: modulino_motors
    id: motor_stepper
    modulino_motors_id: motors1
    max_speed: 250 steps/s
```

---
