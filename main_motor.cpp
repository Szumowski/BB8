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


uint8_t senderAddress[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00}; // <-- ESP32_Main_Led Mac address

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
