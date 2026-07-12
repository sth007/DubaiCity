# ESP32 Dubai Smart Map Demo

A demonstration project for an **ESP32**, **1.3" SH1106 OLED Display** and **5 WS2812B LEDs**.

Each LED represents a famous landmark in Dubai.
The OLED displays the landmark name and its abbreviation while the corresponding LED lights up.

---

# Hardware

## Microcontroller

- ESP32 Dev Module

## Display

- 1.3" OLED
- SH1106 Controller
- 128x64 Pixel
- I2C
- Address: `0x3C`

## LEDs

- 5 × WS2812B / NeoPixel

---

# Required Libraries

Install using **Arduino IDE → Library Manager**

## FastLED

Author:
FastLED

Purpose:
Control WS2812B LEDs.

---

## U8g2

Author:
oliver

Purpose:
Graphics library for SH1106 OLED displays.

---

## Wire

Already included with the ESP32 package.

Used for I2C communication.

---

# Board Settings

Board:

```
ESP32 Dev Module
```

Do **not** select

```
ESP32S3 Dev Module
```

---

# Wiring

## OLED

| OLED | ESP32 |
|-------|--------|
| VDD | 3.3V |
| GND | GND |
| SDA | GPIO21 |
| SCK | GPIO22 |

---

## WS2812B LEDs

| LED | ESP32 |
|------|--------|
| DIN | GPIO18 |
| +5V | 5V |
| GND | GND |

**Important**

All grounds (GND) must be connected together.

---

# Project Structure

The project stores all landmarks inside a two-dimensional array.

Each entry contains:

- Landmark name
- LED number
- Short ID

Example

```cpp
{
    "Burj Khalifa",
    "0",
    "BK"
}
```

---

# Current Landmark Database

| ID | Landmark | LED |
|----|-----------|-----|
| BK | Burj Khalifa | 1 |
| PJ | Palm Jumeirah | 2 |
| BA | Burj Al Arab | 3 |
| DM | Dubai Mall | 4 |
| MA | Dubai Marina | 5 |

---

# Main Function

```cpp
showSight("BK");
```

The function

- searches the landmark by ID
- displays the landmark on the OLED
- switches on the assigned LED
- switches all other LEDs off

---

# Automatic Demo

The `loop()` function cycles through all landmarks automatically.

```cpp
for (uint8_t i = 0; i < kSightCount; i++)
{
    showSight(dubaiSights[i][2]);
    delay(3000);
}
```

---

# Serial Output

Example

```
Dubai Landmark Demo

OLED started.

Showing:
Burj Khalifa

ID: BK
LED: 1
```

---

# Features

- SH1106 OLED support
- FastLED support
- Landmark database
- Lookup by ID
- Automatic demo mode
- Easy to extend
- Clean and modular code

---

# Adding New Landmarks

Simply add another row to the array.

Example

```cpp
{
    "Museum of Future",
    "5",
    "MF"
}
```

Increase

```cpp
kSightCount
```

and connect another LED.

No further code changes are required.

---

# Future Ideas

- GPS position
- Touch buttons
- WiFi API
- Weather information
- RGB colors per landmark
- Animation effects
- Multiple cities
- SD card database
- Web configuration
- OTA firmware updates

---

# License

MIT License
