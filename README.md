# BB8
BB-8 project inspired by Star Wars, built with ESP32 and C++. This wireless-controlled robot features motion, distance sensing, and expandability with AI. The robot uses a VL53L0X time-of-flight sensor for obstacle detection and a simple motor driver setup for movement.

Key Features:

Motion control: Dual motors controlled via ESP32 GPIO pins (IN1–IN4).

Obstacle detection: VL53L0X sensor reads distance in millimeters and adjusts movement automatically.

Autonomous response: Moves forward until an object is detected within 200 mm, then changes direction.

Wireless expandability: Can be extended with Wi-Fi/Bluetooth control and AI modules.

Platform: Developed using Arduino framework with PlatformIO.

Code Overview (main.cpp):

Initializes serial communication and motor pins.

Sets up the VL53L0X sensor for distance measurement.

In the loop, reads the distance and prints it to Serial.

Moves motors forward normally; reverses or changes motor state when an obstacle is detected.

Dependencies (platformio.ini):

ESP32 DoIt Devkit V1 board.

Arduino framework.

VL53L0X sensor library (pololu/VL53L0X@^1.3.1).

Behavior:

Distance ≥ 200 mm: Moves forward normally.

Distance < 200 mm: Adjusts motor pins to avoid collision.

Serial output: Prints measured distance and timeout messages for debugging.
