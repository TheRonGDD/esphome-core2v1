# M5Stack Core2 v1.0 ESPHome Configuration

A **fully functional, burn-in ready** ESPHome configuration for the M5Stack Core2 v1.0 touch-screen device. This template exposes all hardware capabilities and is ready to be customized for your specific use case.

## 🎯 What This Is

This is a **generic template** for anyone who wants to:
- Use an M5Stack Core2 v1.0 as a Home Assistant control panel or smart display
- Build a custom touchscreen interface with multiple pages
- Control lights, fans, switches, and other Home Assistant entities
- Access all device capabilities (IMU, battery, WiFi, backlight, vibration, audio, microphone)
- Have a clean starting point without alarm clock or game logic

## ✨ Features

- **Multi-Page UI** — Three customizable pages (A, B, C) accessible via bezel buttons
- **Home Assistant Integration** — Toggle lights, fans, switches directly from device
- **Full Hardware Exposure** — All Core2 v1.0 capabilities available:
  - 🔆 Backlight control with safety features
  - 📳 Vibration motor
  - 🔋 Battery monitoring (voltage, percentage, charging status)
  - 📡 WiFi signal strength
  - 🎙️ Microphone and speaker (optional voice assistant)
  - 📊 IMU sensors (accelerometer, gyro, temperature)
  - 🕐 Real-time clock from Home Assistant
  - 🌡️ Optional system diagnostics (CPU temp, heap memory, uptime)

- **Commented Examples** — Every optional feature includes working examples
- **Production-Ready** — Safety features enforce backlight minimum to keep touch responsive
- **Clean Codebase** — No alarm clocks, games, or device-specific logic

## 📋 Requirements

- **Hardware:** M5Stack Core2 v1.0 device
- **Software:**
  - ESPHome 2024.1.0 or later
  - Home Assistant (recommended, optional for basic functionality)
- **Network:** WiFi connectivity for OTA updates and HA integration

## 🚀 Quick Start

### 1. Clone This Repository
```bash
git clone https://github.com/yourusername/esphome-core2v1.git
cd esphome-core2v1
```

### 2. Create secrets.yaml
```yaml
# secrets.yaml
wifi_ssid: "Your WiFi SSID"
wifi_password: "Your WiFi Password"
```

### 3. Edit Device Name (Optional)
In `core2v1.yaml`, change the substitutions:
```yaml
substitutions:
  devicename: core2v1          # Change to: my-control-panel
  upper_devicename: Core2 V1   # Change to: My Control Panel
```

### 4. Flash to Device
```bash
# Connect device via USB
esphome run core2v1.yaml

# Or build and manually upload
esphome compile core2v1.yaml
# Then use M5Stack's USB drivers to upload the .bin file
```

### 5. Access Home Assistant
Once flashed and connected to WiFi:
- Open Home Assistant
- Go to Settings → Devices & Services → ESPHome
- The device will auto-discover (or manually add if needed)
- Add to dashboard or customize as needed

## 🎨 Customization Guide

### Adding Controls to Page A

Uncomment the example in `core2v1.yaml` (search for `PAGE A - Generic Example Page`):

```yaml
# Example: Light toggle button
- obj:
    width: 140
    height: 60
    radius: 12
    clickable: true
    bg_color: 0xFF80AB
    on_press:
      - homeassistant.service:
          service: light.toggle
          data:
            entity_id: light.YOUR_LIGHT_ENTITY_ID
    widgets:
      - label:
          text: "Light"
          text_color: 0xFFFFFF
          text_align: CENTER
          width: 140
          height: 60
```

Replace `light.YOUR_LIGHT_ENTITY_ID` with your actual Home Assistant entity ID.

### Enabling Optional Features

**Clock Page (time display):**
1. Uncomment the entire `clock_page:` LVGL definition
2. Uncomment the `on_time:` events in the Time section
3. Uncomment `on_idle:` in the LVGL section

**Device Info Page (WiFi, battery, IP, uptime):**
1. Uncomment the `device_info_page:` definition
2. Uncomment the `update_device_info` script
3. Add to bezel button or on_idle timeout

**Voice Assistant:**
1. Uncomment `microphone:` and `voice_assistant:` sections
2. Uncomment the `on_boot:` voice_assistant.start
3. Uncomment `btn_voice_assist` button
4. Set up a conversation agent in Home Assistant

**System Diagnostics:**
Uncomment any of these sensors in the `sensor:` section:
- CPU Temperature
- Free Heap Memory
- WiFi Signal Strength as %
- Uptime
- Display Brightness Level

### Customizing Button HA Service Calls

Edit the `button:` section to add your own controls:

```yaml
button:
  - platform: template
    name: "My Custom Light"
    id: btn_my_light
    on_press:
      - homeassistant.service:
          service: light.turn_on
          data:
            entity_id: light.living_room
            brightness: 255
```

## 📚 Component Documentation

### AXP192Custom (Power Management)

See `external_components/axp192_custom/README.md` for detailed information on:
- Backlight control and safety features
- Vibration motor control
- Battery monitoring
- Customization examples

**Key Feature:** Prevents device from becoming unresponsive if backlight is turned completely off (enforces safety minimum).

## 🔧 Hardware Details

### I2C Devices
- **AXP192** (address 0x34) — Power management IC
- **MPU6886** (address 0x68) — IMU sensor
- **FT63x6** (interrupt pin GPIO39) — Touchscreen controller

### Display
- **MIPI SPI** — M5Core2 2.0" IPS LCD (320×240)
- **Update interval:** 0s (reactive, no polling)
- **Auto-clear:** Disabled (managed by LVGL)

### Audio
- **I2S Audio Bus** — Speaker output and microphone input
- **Speaker:** External DAC via I2S
- **Microphone:** I2S input, 16kHz sample rate

### GPIO
- **SPI CLK:** GPIO18
- **SPI MOSI:** GPIO23
- **SPI DC:** GPIO15
- **Touchscreen INT:** GPIO39
- **I2C SDA:** GPIO21
- **I2C SCL:** GPIO22
- **I2S LRCLK:** GPIO0
- **I2S BCLK:** GPIO12
- **I2S DOUT:** GPIO2
- **I2S DIN:** GPIO34

## 📊 File Structure

```
esphome/
├── core2v1.yaml                    # Main configuration (903 lines)
├── secrets.example.yaml            # Copy to secrets.yaml
├── README.md                        # This file
├── external_components/
│   └── axp192_custom/              # Custom power management component
│       ├── __init__.py             # Component registration
│       ├── axp192_custom.h         # Header
│       ├── axp192_custom.cpp       # Implementation (safety features)
│       └── README.md               # Component documentation
└── fonts/
    └── materialdesignicons-webfont.ttf  # Icon font
```

## 🆚 What's Different from Original

**Removed:**
- ❌ Alarm clock logic
- ❌ Math game for dismissing alarms
- ❌ Device-specific references (girls room lights, fans)
- ❌ Hard-coded Home Assistant entity IDs
- ❌ Alarm state management

**Added:**
- ✅ Three generic example pages (A, B, C)
- ✅ Extensive comments and boilerplate examples
- ✅ System diagnostics sensors (commented, ready to enable)
- ✅ Clean customization workflow
- ✅ Comprehensive documentation

**Kept:**
- ✅ All hardware capabilities exposed
- ✅ Full Home Assistant integration
- ✅ Production safety features
- ✅ Optional clock page (commented example)
- ✅ Optional voice assistant
- ✅ Optional device info display

## 🐛 Troubleshooting

### Device not responding to touch
- Check that backlight is not at 0 (component enforces minimum)
- Verify USB cable and power
- Check GPIO39 touchscreen interrupt line

### WiFi not connecting
- Verify SSID and password in `secrets.yaml`
- Check PSRAM mode is set to `quad` (required for Core2 v1.0)
- Monitor logs: `esphome logs core2v1.yaml`

### Display not updating
- Verify SPI pins (CLK=GPIO18, MOSI=GPIO23, DC=GPIO15)
- Check LVGL settings are enabled
- Try reducing `update_interval` if very low

### Battery reading incorrect
- Calibrate against known voltage with multimeter
- Linear equation assumes 3.2V = 0%, 4.2V = 100%
- Adjust conversion in `sensor:` battery_percent template if needed

### Voice assistant not working
- Ensure microphone and speaker GPIO pins are correct
- Verify HA conversation agent is configured
- Test audio with media_player first

## 📝 Configuration Tips

### Reduce WLED/LVGL Memory Usage
If experiencing crashes, try:
- Reducing font sizes
- Lowering update intervals
- Disabling optional sensors

### Optimize for Battery
- Set backlight to 50-70 range when plugged in
- Disable vibration motor (save ~50mA)
- Disable voice assistant if not in use
- Reduce WiFi signal sampling interval

### Optimize for Responsiveness
- Keep LVGL update_interval at 0s
- Pre-load frequently used pages
- Minimize lambda computation in on_press handlers

## 🤝 Contributing

Found a bug? Have an improvement?
- Open an issue describing the problem
- Include your Core2 v1.0 revision (usually printed on back)
- Attach relevant logs if applicable

## 📄 License

This configuration and AXP192Custom component are provided as-is for personal and educational use.

The M5Stack Core2 is a product of M5Stack (https://m5stack.com).

## 🔗 Resources

- [M5Stack Core2 Documentation](https://docs.m5stack.com/en/core/core2)
- [ESPHome Documentation](https://esphome.io/)
- [Home Assistant](https://www.home-assistant.io/)
- [LVGL (Display Framework)](https://lvgl.io/)

## 💡 Next Steps

1. **Flash the device** — See Quick Start above
2. **Customize Page A, B, C** — Add your Home Assistant controls
3. **Enable optional features** — Clock, voice, diagnostics
4. **Extend the AXP192Custom component** — Add custom power rails
5. **Share your version** — If you create something cool, let us know!

---

**Questions?** Check the commented examples throughout `core2v1.yaml` — most customizations are shown there.

**Ready to go!** 🚀
