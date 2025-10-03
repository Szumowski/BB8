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

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_RGB + NEO_KHZ800);

VL53L0X sensor;

void setColor(uint8_t r, uint8_t g, uint8_t b) {
  for(int i = 0; i < LED_COUNT; i++) {
    strip.setPixelColor(i, strip.Color(r, g, b));
  }
  strip.show();
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

setColor(0, 0, 255); 

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

} else {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);

    setColor(255, 0, 0); 
}

delay(100);
}   
