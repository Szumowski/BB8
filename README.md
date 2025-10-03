# BB-8

BB-8 project inspired by Star Wars, built with ESP32 and C++.
This wireless-controlled robot features motion, distance sensing, LED feedback with NeoPixel, and expandability with AI.
The robot uses a VL53L0X time-of-flight sensor for obstacle detection, a WS2812 (NeoPixel) LED ring for visual indication, and a simple motor driver setup for movement.

Key Features

Motion control: Dual motors controlled via ESP32 GPIO pins (IN1–IN4).

Obstacle detection: VL53L0X sensor reads distance in millimeters and adjusts movement automatically.

Autonomous response:

Moves forward until an object is detected within 200 mm.

Then changes motor state to avoid collision.

LED feedback (NeoPixel WS2812):

🔵 Blue = system initialization (startup).

🟢 Green = obstacle detected (distance < 200 mm).

🔴 Red = path clear (distance ≥ 200 mm).

Wireless expandability: Can be extended with Wi-Fi/Bluetooth control and AI modules.

Platform: Developed using Arduino framework with PlatformIO.

Code Overview (main.cpp)

Initializes serial communication, motor control pins, VL53L0X sensor, and NeoPixel LEDs.

In the setup:

Motors set to idle state.

VL53L0X initialized with timeout.

NeoPixel ring initialized and set to blue (system ready).

In the loop:

Reads distance from the VL53L0X sensor.

Prints measured distance to Serial.

If distance < 200 mm → sets motors for avoidance and LEDs to green.

If distance ≥ 200 mm → moves forward and LEDs turn red.

If sensor timeout occurs → prints error to Serial (no LED change).

Dependencies (platformio.ini)

Board: ESP32 DoIt Devkit V1

Framework: Arduino

Libraries:

VL53L0X → pololu/VL53L0X@^1.3.1

NeoPixel → adafruit/Adafruit NeoPixel@^1.12.3

Behavior

Startup → LEDs = 🔵 blue (system initialized).

Distance ≥ 200 mm → Moves forward, LEDs = 🔴 red.

Distance < 200 mm → Adjusts motors to avoid collision, LEDs = 🟢 green.

Sensor timeout → Serial prints "Sensor timeout!".
