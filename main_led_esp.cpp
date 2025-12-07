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

uint8_t receiverAddress[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00}; // <--- ESP32_Main_Motor Mac address

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
