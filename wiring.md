# 🔌 Wiring Guide

## Arduino Uno Pin Connections

| Pin | Connected To |
|---|---|
| D4 (RX) | GPS Module TX |
| D3 (TX) | GPS Module RX |
| D8 | Morse Button (other leg → GND) |
| D7 | Control Button (other leg → GND) |
| D12 | LED (+) → 220Ω resistor → GND |
| D11 | Buzzer (+), Buzzer (−) → GND |
| A4 (SDA) | LCD SDA |
| A5 (SCL) | LCD SCL |
| 5V | LCD VCC, GPS VCC |
| GND | LCD GND, GPS GND, Button GND, LED GND, Buzzer GND |

## I²C LCD
- Default address: `0x27`
- If LCD doesn't respond, run an I²C scanner sketch to find the correct address and update `IPA195.ino` line:
  ```cpp
  LiquidCrystal_I2C lcd(0x27, 16, 2);
  ```

## GPS Module (NEO-6M or similar)
- VCC → 5V (or 3.3V depending on module)
- GND → GND
- TX → Arduino D4
- RX → Arduino D3

## NodeMCU ESP8266
- Powers independently via USB or 3.3V regulator
- Phase 2 (future): D10 (Arduino RX) ↔ NodeMCU TX, D9 (Arduino TX) ↔ NodeMCU RX

> **Note:** SoftwareSerial pins for ESP bridge are already declared (commented out) in `IPA195.ino` for Phase 2 use.
