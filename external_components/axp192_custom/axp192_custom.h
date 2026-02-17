#pragma once
#include "esphome/core/component.h"
#include "esphome/components/i2c/i2c.h"
#include <cstdint>
#include "esphome/core/helpers.h"

namespace esphome {
namespace axp192_custom {

class AXP192Custom : public Component, public i2c::I2CDevice {
 public:
  void setup() override;
  float get_setup_priority() const override { return setup_priority::DATA; }

  // Backlight control (0-127 hardware range)
  void set_backlight_level(uint8_t level);

  // Vibration motor
  void set_vibration(bool enable);

  void dump_config() override;

  // Low-level register access (public so axp192_output can use if needed)
  uint8_t read_reg(uint8_t reg);
  void write_reg(uint8_t reg, uint8_t val);

 protected:
  // LCD logic power (must stay on for Core2 v1.0)
  void set_ldo2(bool enable);
};

}  // namespace axp192_custom
}  // namespace esphome
