#include "axp192_custom.h"
#include "esphome/core/log.h"

namespace esphome {
namespace axp192_custom {

static const char *const TAG = "axp192_custom";

// ------------------------------------------------------
// SETUP
// ------------------------------------------------------
void AXP192Custom::setup() {
  ESP_LOGI(TAG, "AXP192 Core2 v1.0 setup starting");

  // Configure power output enables (0x12)
  uint8_t reg = read_reg(0x12);
  reg |= 0x04;   // LDO1 always on (RTC, etc.)
  reg &= ~0x08;  // Vibration LDO3 off by default
  reg |= 0x10;   // LDO2 ON - LCD logic/peripherals (CRITICAL for Core2 v1.0!)
  write_reg(0x12, reg);

  // Set initial backlight to medium brightness
  set_backlight_level(92);  // ~3.0V - good default

  ESP_LOGI(TAG, "PMU reg 0x12 (power enables): 0x%02X", read_reg(0x12));
  ESP_LOGI(TAG, "Backlight DCDC3 voltage (0x27): 0x%02X", read_reg(0x27));
  ESP_LOGCONFIG(TAG, "AXP192Custom component initialized");
}

// ------------------------------------------------------
// BACKLIGHT CONTROL (using DCDC3 register 0x27)
// ------------------------------------------------------
void AXP192Custom::set_backlight_level(uint8_t level) {
  // AXP192 DCDC3 range: 0-127
  // CRITICAL for Core2 v1.0: DCDC3 powers touch controller AND backlight!
  // Setting to 0 will make device unresponsive - enforce minimum
  const uint8_t SAFETY_MINIMUM = 50;  // Never go below this
  
  if (level > 127) level = 127;
  if (level < SAFETY_MINIMUM && level != 0) level = SAFETY_MINIMUM;
  
  // Only allow 0 if explicitly requested AND log warning
  if (level == 0) {
    ESP_LOGW(TAG, "Backlight level 0 requested - this may power off touch controller!");
    ESP_LOGW(TAG, "Device may become unresponsive. Consider using level >= %d", SAFETY_MINIMUM);
  }
  
  write_reg(0x27, level);
  ESP_LOGD(TAG, "Backlight set to level: %d", level);
}

// ------------------------------------------------------
// VIBRATION MOTOR (LDO3)
// ------------------------------------------------------
void AXP192Custom::set_vibration(bool enable) {
  uint8_t reg = read_reg(0x12);
  if (enable)
    reg |= 0x08;   // LDO3 ON
  else
    reg &= ~0x08;  // LDO3 OFF
  write_reg(0x12, reg);
  ESP_LOGD(TAG, "Vibration motor: %s", enable ? "ON" : "OFF");
}

// ------------------------------------------------------
// REGISTER ACCESS
// ------------------------------------------------------
uint8_t AXP192Custom::read_reg(uint8_t reg) {
  uint8_t val = 0;
  read_byte(reg, &val);
  return val;
}

void AXP192Custom::write_reg(uint8_t reg, uint8_t val) {
  write_byte(reg, val);
}

// ------------------------------------------------------
// LCD LOGIC POWER (LDO2) - kept for completeness
// ------------------------------------------------------
void AXP192Custom::set_ldo2(bool enable) {
  uint8_t reg = read_reg(0x12);
  if (enable)
    reg |= 0x10;   // bit 4 = LDO2
  else
    reg &= ~0x10;
  write_reg(0x12, reg);
  ESP_LOGW(TAG, "LDO2 changed to %s - this should normally stay ON for Core2 v1.0!", 
           enable ? "ON" : "OFF");
}

// ------------------------------------------------------
// CONFIG DUMP
// ------------------------------------------------------
void AXP192Custom::dump_config() {
  ESP_LOGCONFIG(TAG, "AXP192Custom:");
  ESP_LOGCONFIG(TAG, "  Address: 0x%02X", this->address_);
  ESP_LOGCONFIG(TAG, "  Core2 v1.0 mode");
}

}  // namespace axp192_custom
}  // namespace esphome
