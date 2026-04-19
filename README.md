# 📡 Morse Code GPS Communicator

A two-board Arduino project that lets you type messages using Morse code via a push button, display them on an I²C LCD, capture your GPS location, and send push notifications via a NodeMCU ESP8266 + Pushover.

---

## 🔧 Hardware

| Component | Qty | Notes |
|---|---|---|
| Arduino Uno (or compatible) | 1 | Main controller |
| NodeMCU ESP8266 | 1 | Wi-Fi + Pushover notifications |
| GPS Module (e.g. NEO-6M) | 1 | SoftwareSerial on pins 4 (RX), 3 (TX) |
| 16×2 I²C LCD | 1 | Address `0x27` |
| Push button (Morse) | 1 | Pin 8 |
| Push button (Control) | 1 | Pin 7 |
| LED | 1 | Pin 12 |
| Buzzer | 1 | Pin 11 |

---

## 📁 Project Structure

```
MorseCode-GPS-Communicator/
├── arduino/
│   └── IPA195/
│       └── IPA195.ino          ← Main Arduino sketch
├── nodemcu/
│   └── NodeMCU.ino             ← ESP8266 Wi-Fi + Pushover sketch
├── docs/
│   └── wiring.md               ← Wiring guide
├── .gitignore
└── README.md
```

---

## ⚙️ How It Works

### Arduino (IPA195.ino)
- **Morse Button (pin 8):** Hold to generate dots/dashes. Short press = dot, longer press = dash.
- **Control Button (pin 7):**
  - Short press → Backspace
  - Hold ~2 s → Add space
  - Hold ~5 s → Clear screen
- **GPS:** On the first Morse button press each session, the current GPS location is printed to Serial as a Google Maps link.
- **LCD:** Decoded text is displayed in real-time on the 16×2 LCD.
- **LED + Buzzer:** Give audio/visual feedback while a button is held.

### NodeMCU (NodeMCU.ino)
- Connects to Wi-Fi on boot.
- Sends a Pushover push notification to your phone confirming it's online.
- Future phase: receive decoded Morse messages from Arduino via SoftwareSerial and relay them as notifications.

---

## 🚀 Getting Started

### 1. Clone the repo
```bash
git clone https://github.com/YOUR_USERNAME/MorseCode-GPS-Communicator.git
```

### 2. Install Arduino Libraries
Install the following via Arduino IDE → **Sketch → Include Library → Manage Libraries**:

| Library | Used In |
|---|---|
| `LiquidCrystal_I2C` | IPA195.ino |
| `TinyGPS++` | IPA195.ino |
| `SoftwareSerial` | IPA195.ino (built-in) |
| `ESP8266WiFi` | NodeMCU.ino (ESP8266 core) |
| `ESP8266HTTPClient` | NodeMCU.ino |

### 3. Configure NodeMCU credentials
Open `nodemcu/NodeMCU.ino` and fill in your values:
```cpp
const char* ssid     = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";

const char* pushoverUserKey  = "YOUR_PUSHOVER_USER_KEY";
const char* pushoverAPIToken = "YOUR_PUSHOVER_API_TOKEN";
```
> ⚠️ **Never commit real credentials to GitHub.** Consider using a `secrets.h` file (already in `.gitignore`).

### 4. Upload
- Upload `IPA195.ino` to your **Arduino Uno** (board: Arduino Uno, port: your COM port).
- Upload `NodeMCU.ino` to your **NodeMCU ESP8266** (board: NodeMCU 1.0 (ESP-12E Module), CPU: 80 MHz).

---

## 🔤 Morse Code Timing

| Input | Duration |
|---|---|
| Dot | ≥ 100 ms |
| Dash | ≥ 250 ms |
| Letter decode timeout | 3000 ms of inactivity |
| Space (control button hold) | ≥ 2000 ms |
| Clear screen (control button hold) | ≥ 5000 ms |

---

## 📦 Libraries Folder

The `libraries/` folder in the original project contains local copies of all required libraries. You do **not** need to include these in the repo — install them via the Arduino Library Manager instead (see above). If you need a specific version, note it in this README.

---

## 🛣️ Roadmap

- [x] Morse input with dot/dash detection
- [x] 16×2 LCD real-time display
- [x] GPS location on Serial
- [x] NodeMCU Wi-Fi + Pushover notification
- [ ] Arduino → NodeMCU serial bridge (Phase 2)
- [ ] Send decoded Morse sentence as Pushover notification
- [ ] OLED display upgrade

---

