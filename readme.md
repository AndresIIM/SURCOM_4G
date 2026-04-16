# SURCOM_4G

## 📖 Description
Embedded system for data acquisition and communication based on ESP32.
The system is designed to:
- Sensor (ph Honeywell Durafet III)
- Read sensors (ADS1220, thermistor, etc.)
- Monitor battery status (INA219)
- Store data locally (SD card)
- Communicate via LoRa / 4G
- Provide local configuration (WiFi + web interface)

---

## 🧰 Hardware
- ESP32 (LilyGO T7000G & SD)
- ADS1220 (ADC for pH / sensors)
- INA219 (battery monitoring)
- RTC (DS3231)
- SD card

---

## ⚙️ Features
- Real-time data acquisition
- Data logging (CSV / structured format)
- Remote communication (4G)
- Local web interface for monitoring and calibration
- Serial interface for debugging and configuration

---

## 📂 Project Structure

```text
SURCOM_4G/
├── FIRMWARE/    → PlatformIO project
├── HARDWARE/    → schematics, PCB, BOM
├── DOCS/        → documentation, manuals

