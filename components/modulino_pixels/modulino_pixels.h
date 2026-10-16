#pragma once

#include "esphome/core/component.h"
#include "esphome/components/light/addressable_light.h"
#include "esphome/components/light/light_output.h"

#include <Arduino_Modulino.h>

// Thin wrapper around Arduino's own ModulinoPixels class: a strip of 8
// APA102-style addressable RGB LEDs on I2C address 0x6C. Modeled on ESPHome's
// own small I2C addressable strip (esphome/components/m5stack_8angle/light):
// ESPHome owns the RGB framebuffer and, on each write_state(), pushes it into
// the library's set()/show() calls rather than re-deriving the LED protocol.

namespace esphome {
namespace modulino_pixels {

static const uint8_t MODULINO_PIXELS_NUM_LEDS = 8;
static const uint8_t MODULINO_PIXELS_BYTES_PER_LED = 4;

class ModulinoPixelsLightOutput : public light::AddressableLight {
 public:
  void setup() override;
  void write_state(light::LightState *state) override;
  void dump_config() override;

  int32_t size() const override { return MODULINO_PIXELS_NUM_LEDS; }
  light::LightTraits get_traits() override {
    auto traits = light::LightTraits();
    traits.set_supported_color_modes({light::ColorMode::RGB});
    return traits;
  }
  void clear_effect_data() override {
    memset(this->effect_data_, 0x00, MODULINO_PIXELS_NUM_LEDS);
  }

 protected:
  light::ESPColorView get_view_internal(int32_t index) const override;

  ModulinoPixels pixels_;
  bool found_{false};
  uint8_t *buf_{nullptr};
  uint8_t *effect_data_{nullptr};
};

}  // namespace modulino_pixels
}  // namespace esphome
