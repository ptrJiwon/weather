# Weather

<br>

Weather is a system built with ESP32. 
It displays Temperature, Humidity, and City information using the OpenWeatherMap API on a TFT LCD.

<br>

---

## Development Environment

- **Board**: ESP32  
- **IDE**: Arduino IDE  
- **Display**: TFT LCD ILI9341 (320x240)  
- **Libraries**: TFT_eSPI, SPI, HTTPClient, ArduinoJson

<br>

---

## TFT Display Pins (GPIO)  
> **Note:** If you are using the **TFT_eSPI** library, make sure to define the display pins in the library (User_Setup.h).

| Function | Pin |
|----------|-----|
| CS       | 8   |
| DC       | 5   |
| RST      | 7   |
| MOSI     | 38  |
| MISO     | 47  |
| SCLK     | 48  |

<br>

---

## USER Guide

> **Note:** You have to set your WiFi, and OpenWeatherMap API Key.

````cpp
const char* ssid = "YOUR_WIFI_NAME";
const char* password = "YOUR_WIFI_PASSWORD";

http.begin("http://api.openweathermap.org/data/2.5/weather?lat=90&lon=135&appid=YOUR_API_KEY&units=metric");
