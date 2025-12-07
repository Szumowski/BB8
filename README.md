# BB-8 Inspired ESP32 Robot 🤖

In the new version of the project, the previous single-ESP32 architecture has been completely replaced. The system now uses two ESP32 modules communicating via ESP-NOW, which significantly improves reliability, simplifies wiring, and allows the physical separation of system functions.

**System Split Into Two ESP32 Modules**

Developed in **PlatformIO** using the **Arduino framework**.

---

## Features ✨

### 1. ESP32 “LED + Sensor” – located in the head

This module is responsible for:

- measuring the distance using the VL53L0X sensor,

- controlling the NeoPixel LED ring inside the head (changing color depending on distance),

- transmitting the measured distance to the second ESP32.

It acts as the transmitter, sending a **DistanceData** structure with the distance value.

This behavior is implemented in:
### `main_led_esp.cpp`

```cpp
#include <Arduino.h>
#include <Wire.h>
#include <VL53L0X.h>
#include <adafruit_neopixel.h>
#include <esp_now.h>
#include <WiFi.h>

#define LED_PIN 4
#define LED_COUNT 12

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_RGB + NEO_KHZ800);

VL53L0X sensor;

uint8_t receiverAddress[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00}; // <-- ESP32 Mac addres

struct DistanceData {
  uint16_t distance;
};

DistanceData dataToSend;

void setColor(uint8_t r, uint8_t g, uint8_t b){
  for (int i = 0; i < LED_COUNT; i++) strip.setPixelColor(i, strip.Color(r, g, b));
  strip.show();
}

void onSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("Wysyłam Dane: ");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "OK" : "BŁĄD");
}


void setup(){
  Serial.begin(9600);
  Wire.begin(21, 22);

  strip.begin(); strip.show();

  setColor(0, 0, 255);

  if(!sensor.init()){
    Serial.println("Błąd inicjalizacji czujnika!");
    while(1);
  }
  sensor.setTimeout(500);

  WiFi.mode(WIFI_STA);
  if (esp_now_init() != ESP_OK) {
    Serial.println("Błąd inicjalizacji ESP-NOW");
    while (true);
  }

  esp_now_register_send_cb(onSent);

  esp_now_peer_info_t peerInfo = {};
  memcpy(peerInfo.peer_addr, receiverAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Błąd dodawania odbiorcy");
    return;
  }

  Serial.println("System Gotowy!");

}


void loop() {

  uint16_t distance = sensor.readRangeSingleMillimeters();

  if (sensor.timeoutOccurred()) {
    Serial.println("Sensor timeout!");
    delay(500);
    return;
  }

  Serial.printf("Odległość: %d mm\n", distance);
  dataToSend.distance = distance;

  esp_now_send(receiverAddress, (uint8_t *)&dataToSend, sizeof(dataToSend));

  if (distance < 200) {
    setColor(255, 0, 0);
  } else {
    setColor(0, 0, 255);
  }

  delay(500);   
}

```
The module:

  - reads the current distance,

  - updates the LED color (red <200 mm, blue >200 mm),

  - sends the data to the ESP32 located inside the large sphere.

### 2. ESP32 “Motor Controller” – located in the main large sphere

The second ESP32 receives the distance data sent from the head and controls the motors based on the received value.

This is implemented in:

### `main_moto.cpp`

```cpp
#include <Arduino.h>
#include <esp_now.h>
#include <WiFi.h>

#define IN1 5
#define IN2 18
#define IN3 19
#define IN4 21


struct DistanceData {
  uint16_t distance;
};

DistanceData receivedData;


uint8_t senderAddress[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00}; // <-- ESP_Led Mac address

void onReceive(const uint8_t *mac, const uint8_t *incomingData, int len) {
  memcpy(&receivedData, incomingData, sizeof(receivedData));
  Serial.printf("Odebrano odległość: %d mm\n", receivedData.distance);

  if (receivedData.distance < 200) {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
  } else {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
  }
}

void setup() {
  Serial.begin(9600);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("Błąd inicjalizacji ESP-NOW");
    return;
  }


  esp_now_peer_info_t peerInfo = {};
  memcpy(peerInfo.peer_addr, senderAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Błąd dodawania PEER");
    return;
  }

  esp_now_register_recv_cb(onReceive);

  Serial.println("ESP32 MOTOR – gotowy do odbioru!");
}

void loop() {
  delay(100);
}

```

**Responsibilities:**

  - receiving ESP-NOW packets,

  - reading the distance value,

  - controlling motor direction using pins IN1–IN4.

**Logic example:**
If the distance < 200 mm → specific motor reaction (e.g., stop or avoidance).
If ≥ 200 mm → motors operate normally.


**Benefits of the Updated Architecture**

  - no need for long wires between the head and the sphere,

  - LEDs and sensor in the head work independently,

  - the motor controller stays isolated from LED/sensor noise,

  - modular design simplifies maintenance and upgrades,

  - ESP-NOW ensures fast and stable wireless communication.
