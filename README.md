# 🛜 ESP32-Captive-Portal

![ESP32](https://img.shields.io/badge/ESP32-Captive--Portal-blue.svg)
![License](https://img.shields.io/badge/license-MIT-green.svg)
![GeckCode](https://img.shields.io/badge/by-GeckCode-8A2BE2.svg)

> 📡 A simple yet effective Captive Portal for the ESP32 microcontroller  
> 🦎 Created by **GeckCode** – "Geck as in gecko, Code as in programming."

---

## 🚀 Overview

This project creates a **captive portal** using an **ESP32** device. When users connect to the ESP32’s Wi-Fi hotspot, they are automatically redirected to a custom web page. This can be used for:

- ✅ Demos & presentations
- ✅ Local configuration pages
- ✅ Pranks, jokes, or splash pages
- ✅ Educational or onboarding setups

---

## 📷 Preview

<img src="https://user-images.githubusercontent.com/your-preview.png" width="600" alt="Captive Portal Screenshot">

---

## 🛠 Features

- 🌐 Creates a standalone Wi-Fi Access Point
- 🔁 Redirects all DNS queries to the ESP32
- 🧭 Captive Portal triggers on most modern devices
- ⚙️ Custom HTML splash page (editable)
- 🔒 No internet required

---

## 📦 Requirements

- ✅ **ESP32 Dev Module**
- ✅ Arduino IDE / PlatformIO
- ✅ `WiFi.h`, `DNSServer.h`, `WebServer.h`
- ✅ Patience and a good USB cable 😉

---

## 🧪 Installation

### 🧰 Using Arduino IDE

1. Install the **ESP32 board support** from the [Espressif GitHub repo](https://github.com/espressif/arduino-esp32).
2. Clone or download this repository.
3. Open the `.ino` file in Arduino IDE.
4. Select your board (`ESP32 Dev Module`) and correct port.
5. Click **Upload**.

### 🖥 Using PlatformIO

```bash
git clone https://github.com/GeckCode/ESP32-Captive-Portal.git
cd ESP32-Captive-Portal
pio run -t upload
