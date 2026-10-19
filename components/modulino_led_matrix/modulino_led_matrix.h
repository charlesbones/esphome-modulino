#pragma once

#include "esphome/core/component.h"
#include "esphome/components/display/display_buffer.h"

#include <Modulino_LED_Matrix.h>

// Thin wrapper around Arduino's own ModulinoLEDMatrix class (12x8 monochrome
// matrix, I2C 0x39), exposed as an ESPHome display. ESPHome owns the pixel
// buffer (drawn by the user's display lambda) and, each update(), pushes it to
// the matrix through the library's own pixel/render calls.

namespace esphome {
namespace modulino_led_matrix {

class ModulinoLedMatrix : public display::DisplayBuffer {
 public:
  void setup() override;
  void update() override;
  void dump_config() override;

  display::DisplayType get_display_type() override { return display::DisplayType::DISPLAY_TYPE_BINARY; }

 protected:
  void draw_absolute_pixel_internal(int x, int y, Color color) override;
  int get_width_internal() override { return 12; }
  int get_height_internal() override { return 8; }
  size_t get_buffer_length_();

  ModulinoLEDMatrix matrix_;
  bool found_{false};
};

}  // namespace modulino_led_matrix
}  // namespace esphome
