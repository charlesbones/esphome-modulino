#include "modulino_pixels.h"
#include "esphome/core/helpers.h"
#include "esphome/core/log.h"

namespace esphome {
namespace modulino_pixels {

static const char *const TAG = "modulino_pixels";

// ESPHome already folds master brightness and color correction into the RGB
// bytes it stores in buf_, so the LEDs are driven at the APA102 global-current
// maximum and the per-channel values carry the dimming.
static const uint8_t MODULINO_PIXELS_GLOBAL_BRIGHTNESS = 100;

void ModulinoPixelsLightOutput::setup() {
  RAMAllocator<uint8_t> allocator;
  this->buf_ = allocator.allocate(MODULINO_PIXELS_NUM_LEDS * MODULINO_PIXELS_BYTES_PER_LED);
  if (this->buf_ == nullptr) {
    ESP_LOGE(TAG, "Failed to allocate LED buffer");
    this->mark_failed();
    return;
  }
  memset(this->buf_, 0x00, MODULINO_PIXELS_NUM_LEDS * MODULINO_PIXELS_BYTES_PER_LED);

  this->effect_data_ = allocator.allocate(MODULINO_PIXELS_NUM_LEDS);
  if (this->effect_data_ == nullptr) {
    ESP_LOGE(TAG, "Failed to allocate effect data");
    this->mark_failed();
    return;
  }
  memset(this->effect_data_, 0x00, MODULINO_PIXELS_NUM_LEDS);

  // Bring the Arduino Wire bus up on the Modulino/Qwiic pins before the library
  // starts it (the library's own Wire.begin() then keeps these pins).
  Wire.begin(MODULINO_I2C_SDA, MODULINO_I2C_SCL);
  Modulino.begin();
  this->found_ = this->pixels_.begin();
  if (!this->found_) {
    ESP_LOGW(TAG, "Modulino Pixels not found on the I2C bus yet — will keep retrying");
  }
}

void ModulinoPixelsLightOutput::write_state(light::LightState *state) {
  if (!this->found_) {
    this->found_ = this->pixels_.begin();
    if (!this->found_) return;
    ESP_LOGI(TAG, "Modulino Pixels found");
  }
  for (int i = 0; i < MODULINO_PIXELS_NUM_LEDS; i++) {
    size_t pos = i * MODULINO_PIXELS_BYTES_PER_LED;
    this->pixels_.set(i, this->buf_[pos], this->buf_[pos + 1], this->buf_[pos + 2],
                      MODULINO_PIXELS_GLOBAL_BRIGHTNESS);
  }
  this->pixels_.show();
}

light::ESPColorView ModulinoPixelsLightOutput::get_view_internal(int32_t index) const {
  size_t pos = index * MODULINO_PIXELS_BYTES_PER_LED;
  // red, green, blue, white (unused), effect_data, color_correction
  return {this->buf_ + pos, this->buf_ + pos + 1,       this->buf_ + pos + 2,
          nullptr,          this->effect_data_ + index, &this->correction_};
}

void ModulinoPixelsLightOutput::dump_config() {
  ESP_LOGCONFIG(TAG, "Modulino Pixels:");
  ESP_LOGCONFIG(TAG, "  LEDs: %u", MODULINO_PIXELS_NUM_LEDS);
  if (!this->found_) {
    ESP_LOGCONFIG(TAG, "  Communication with Modulino Pixels failed!");
  }
}

}  // namespace modulino_pixels
}  // namespace esphome
