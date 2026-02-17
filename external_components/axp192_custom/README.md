# AXP192Custom Component for M5Stack Core2 v1.0

## Overview

**AXP192Custom** is an ESPHome external component that provides safe, controlled access to the AXP192 power management IC on the M5Stack Core2 v1.0 device. This component is **production-ready** and handles the critical hardware requirements specific to the Core2 v1.0 design.

## What It Does

### 1. **Backlight Control (DCDC3)**
Controls the display brightness via the AXP192's DCDC3 voltage regulator.

**Why it's critical:** On Core2 v1.0, the DCDC3 rail powers **both**:
- The LED backlight
- The touch screen controller

**The Problem:** If DCDC3 is set to 0V (off), the touch controller loses power and the device becomes **completely unresponsive**. You cannot interact with it at all, even though the CPU is still running.

**The Solution:** AXP192Custom enforces a **safety minimum** of 50 (out of 0-127 range), which keeps approximately 2.8V on the rail. This ensures:
- Touch screen always remains responsive
- Users can still interact with the device
- Backlight can be dimmed but never truly off
- ✅ Safe default behavior out of the box

### 2. **Vibration Motor Control (LDO3)**
Enables/disables the vibration/haptic feedback motor via LDO3 power rail.

Exposed to Home Assistant as a toggle switch, allowing:
- Haptic feedback on button press
- Notifications with vibration
- User interaction feedback

### 3. **Battery Monitoring**
Low-level register access for reading battery voltage (registers 0x78/0x79).

Exposes:
- Raw voltage (in volts)
- Calculated battery percentage (3.2V-4.2V range)
- Charging status detection

### 4. **Register Access**
Public `read_reg()` and `write_reg()` methods for direct AXP192 control.

Allows advanced users to:
- Read status registers for diagnostics
- Implement custom power management logic
- Access other AXP192 functions not yet exposed

## Component Status

✅ **Fully Functional** — This component is production-ready and extensively tested on Core2 v1.0 hardware.

✅ **Burn-In Ready** — All critical safety features are implemented. The device can be deployed as-is or customized.

✅ **Ready for Customization** — The code is clean, well-commented, and structured for extension. You can:
- Add new power rail controls (LDO1, LDO4, etc.)
- Implement additional AXP192 features (GPIO, temperature monitoring)
- Add real-time diagnostics or logging
- Create custom power profiles

## File Structure

```
axp192_custom/
├── __init__.py          # ESPHome component registration and configuration
├── axp192_custom.h      # C++ header with class definition
├── axp192_custom.cpp    # C++ implementation with safety logic
└── README.md            # This file
```

## Usage in core2v1.yaml

The component is registered and used in the main YAML as:

```yaml
axp192_custom:
  id: axp192_custom_id
  i2c_id: bus_a
  address: 0x34
```

Then referenced throughout the YAML:
- **Backlight:** `id(axp192_custom_id).set_backlight_level(92)`
- **Vibration:** `id(axp192_custom_id).set_vibration(true/false)`
- **Battery voltage:** `id(axp192_custom_id).read_reg(0x78)`

## Key Safety Features

### 1. Backlight Safety Minimum
- **Min safe level:** 50 (out of 0-127)
- **Maps to voltage:** ~2.8V (keeps touch responsive)
- **Startup default:** 92 (~3.0V, good visibility)
- **Warning logged if 0 is requested:** Explains the risk to users

### 2. LDO2 Critical Power
- **Always enabled on startup** (bit 4 of register 0x12)
- **Powers LCD logic circuits and touch controller**
- **Must remain ON for device to function**
- Method provided but not recommended for use

### 3. Safe Defaults
- LDO1 always ON (RTC and internal logic)
- LDO3 off by default (vibration motor)
- LDO2 ON (LCD logic)

## Customization Examples

### Add Control for Another Power Rail (LDO4)

```cpp
// In axp192_custom.h:
void set_ldo4(bool enable);

// In axp192_custom.cpp:
void AXP192Custom::set_ldo4(bool enable) {
  uint8_t reg = read_reg(0x12);
  if (enable)
    reg |= 0x20;   // bit 5 = LDO4
  else
    reg &= ~0x20;
  write_reg(0x12, reg);
}
```

Then expose in YAML as a switch.

### Monitor AXP192 Temperature

The AXP192 has an internal temperature sensor (register 0x5E):

```cpp
// In axp192_custom.cpp:
float AXP192Custom::read_temperature() {
  uint8_t temp_raw = read_reg(0x5E);
  return (float)temp_raw - 70.0f;  // Rough conversion
}
```

### Implement Power Profiles

Create methods for different power modes:

```cpp
void AXP192Custom::set_power_mode_low() {
  set_backlight_level(50);      // Minimum brightness
  set_vibration(false);          // Disable haptics
  ESP_LOGI(TAG, "Low power mode enabled");
}

void AXP192Custom::set_power_mode_normal() {
  set_backlight_level(92);       // Normal brightness
  set_vibration(true);           // Haptics enabled
  ESP_LOGI(TAG, "Normal mode enabled");
}
```

## Hardware Reference

**AXP192 I2C Address:** 0x34 (fixed)

**Key Registers Used:**
- **0x12** — Power control (LDO enables, DCDC enables)
- **0x27** — DCDC3 output voltage (backlight)
- **0x5E** — Internal temperature
- **0x78/0x79** — Battery voltage

**Bus:** I2C (GPIO21=SDA, GPIO22=SCL, 400kHz)

## Troubleshooting

**Device becomes unresponsive when backlight is turned off:**
- This is expected behavior — DCDC3 powers the touch controller
- Set backlight to minimum (50) instead of 0
- Component enforces this, but HA might still request level 0

**Vibration doesn't work:**
- Check that LDO3 is enabled (register 0x12, bit 3)
- Verify motor connections (M5Stack Core2 v1.0 has motor)
- Test with: `id(axp192_custom_id).set_vibration(true)`

**Battery percentage reads incorrectly:**
- Raw voltage calculation uses 3.2V-4.2V linear range
- May need calibration for your battery chemistry
- Check actual voltage with multimeter

## License

Part of the OpenClaw Core2v1 ESPHome template. Provided as-is for educational and personal use.

## Further Reading

- [AXP192 Datasheet](https://datasheetspdf.com/pdf/1217098/XPowers/AXP192/1) (external reference)
- [M5Stack Core2 Documentation](https://docs.m5stack.com/en/core/core2)
- [ESPHome Component Development](https://esphome.io/custom/custom_component.html)
