# BB-8 Inspired ESP32 Robot 🤖

Autonomous robot inspired by **BB-8** from *Star Wars*.  
Built with **ESP32**, **VL53L0X** distance sensor, and **NeoPixel (WS2812)** LEDs for motion, obstacle detection, and visual feedback.

Developed in **PlatformIO** using the **Arduino framework**.

---

## Features ✨

### Motion Control
- Dual DC motors controlled via ESP32 GPIO:
  - IN1 – GPIO 5
  - IN2 – GPIO 18
  - IN3 – GPIO 16
  - IN4 – GPIO 17
- Moves forward when path is clear.
- Changes direction to avoid obstacles (<200 mm).

### Obstacle Detection
- VL53L0X Time-of-Flight sensor.
- Distance range: 30–2000 mm.
- Timeout prints `Sensor timeout!` in Serial Monitor.

### LED Feedback (NeoPixel WS2812)
Two LED rings provide visual status:

| System State | Condition | LED Color (GRB) | Meaning |
|--------------|-----------|-----------------|---------|
| 🔵 Blue | Startup | (0, 0, 255) | System ready / initialization |
| 🟢 Green | Distance ≥ 200 mm | (0, 255, 0) | Path clear – moving forward |
| 🔴 Red | Distance < 200 mm | (255, 0, 0) | Obstacle detected – avoidance |

> **Note:** WS2812 LEDs use **GRB** color order, not RGB. Colors in the code are adjusted accordingly.

---

## How It Works ⚙️

1. **Setup**
   - Starts serial communication (`Serial.begin(9600)`).
   - Configures motor pins as outputs.
   - Initializes I²C (SDA = GPIO 21, SCL = GPIO 22).
   - Initializes VL53L0X sensor (timeout = 500 ms).
   - Initializes NeoPixel LEDs and sets them to **blue**.
   - Prints `System Ready!` to Serial Monitor.

2. **Loop**
   - Reads distance from the sensor.
   - If timeout → prints `Sensor timeout!` and skips loop.
   - If distance < 200 mm:
     - Motors change direction (avoidance).
     - LEDs turn **red**.
   - If distance ≥ 200 mm:
     - Robot moves forward.
     - LEDs turn **green**.
   - Loop delay: 100 ms.

---

## Code Structure 📂

### `main.cpp`

```cpp
#include <Arduino.h> 
#include <Wire.h>
#include <VL53L0X.h>
#include <Adafruit_NeoPixel.h>

#define IN1 5 
#define IN2 18 
#define IN3 16
#define IN4 17 

#define LED_PIN 4
#define LED_COUNT 16

#define LED_PIN2 2
#define LED_COUNT2 12

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_RGB + NEO_KHZ800);
Adafruit_NeoPixel strip2(LED_COUNT2, LED_PIN2, NEO_RGB + NEO_KHZ800);

VL53L0X sensor;

void setColor(uint8_t r, uint8_t g, uint8_t b) {
  for (int i = 0; i < LED_COUNT; i++) {
    strip.setPixelColor(i, strip.Color(r, g, b));
  }
  strip.show();
}

void setColor2(uint8_t r, uint8_t g, uint8_t b) {
  for (int i = 0; i < LED_COUNT2; i++) {
    strip2.setPixelColor(i, strip2.Color(r, g, b));
  }
  strip2.show();
}

void setup(){ 

Serial.begin(9600);

pinMode(IN1, OUTPUT); 
pinMode(IN2, OUTPUT); 
pinMode(IN3, OUTPUT); 
pinMode(IN4, OUTPUT);

digitalWrite(IN1, HIGH); 
digitalWrite(IN2, HIGH);
digitalWrite(IN3, HIGH); 
digitalWrite(IN4, HIGH);

Wire.begin(21, 22);

if(!sensor.init()){
    Serial.println("Failed to detect and initialize sensor!");
    while(1);  
}

sensor.setTimeout(500);

  strip.begin();
  strip.show();
  strip2.begin();
  strip2.show();


setColor(0, 0, 255);
setColor2(0, 0, 255);

Serial.println("System Gototwy!");

} 

void loop(){ 
    uint16_t distance = sensor.readRangeSingleMillimeters();

    if(sensor.timeoutOccurred()){
        Serial.println("Sensor timeout!");
        return;
    } 

    Serial.printf("Odległość: %d mm\n", distance);
    
if (distance <200){
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);

    setColor(0, 255, 0); 
    setColor2(0, 255, 0); 

} else {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);

    setColor(0, 0, 0);
    setColor2(0, 255, 0);  
}

delay(100);
}   

