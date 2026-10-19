#include "modulino_led_matrix.h"
#include "esphome/core/log.h"

#include <Arduino_Modulino.h>

namespace esphome {
namespace modulino_led_matrix {

static const char *const TAG = "modulino_led_matrix";

size_t ModulinoLedMatrix::get_buffer_length_() {
  return size_t(this->get_width_internal()) * size_t(this->get_height_internal()) / 8u;
}

void ModulinoLedMatrix::setup() {
  // Bring the Arduino Wire bus up on the Modulino/Qwiic pins before the library
  // starts it (the library's own Wire.begin() then keeps these pins).
  Wire.begin(MODULINO_I2C_SDA, MODULINO_I2C_SCL);
  Modulino.begin();
  this->init_internal_(this->get_buffer_length_());
  found_ = matrix_.begin();
  if (!found_) {
    ESP_LOGW(TAG, "Modulino LED Matrix not found on the I2C bus yet — will keep retrying");
  }
}

void ModulinoLedMatrix::update() {
  if (!found_) {
    found_ = matrix_.begin();
    if (!found_) return;
    ESP_LOGI(TAG, "Modulino LED Matrix found");
  }
  this->do_update_();  // runs the user's display lambda / pages into buffer_
  // Push the ESPHome buffer to the matrix via the library (it owns the x/y
  // mapping, so orientation matches the library's own sketches).
  matrix_.beginDraw();
  matrix_.clear();
  for (int x = 0; x < this->get_width_internal(); x++) {
    for (int y = 0; y < this->get_height_internal(); y++) {
      if (this->buffer_[x] & (1 << y)) {
        matrix_.set(x, y, 255, 255, 255);
      }
    }
  }
  matrix_.endDraw();
}

void ModulinoLedMatrix::draw_absolute_pixel_internal(int x, int y, Color color) {
  if (x < 0 || x >= this->get_width_internal() || y < 0 || y >= this->get_height_internal()) {
    return;
  }
  if (color.is_on()) {
    this->buffer_[x] |= (1 << y);
  } else {
    this->buffer_[x] &= ~(1 << y);
  }
}

void ModulinoLedMatrix::dump_config() {
  ESP_LOGCONFIG(TAG, "Modulino LED Matrix:");
  ESP_LOGCONFIG(TAG, "  Size: 12x8");
  if (!found_) {
    ESP_LOGCONFIG(TAG, "  Communication with Modulino LED Matrix failed!");
  }
}

}  // namespace modulino_led_matrix
}  // namespace esphome
