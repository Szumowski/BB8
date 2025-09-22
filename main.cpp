#include <Arduino.h> 

#define IN1 5 
#define IN2 18 
#define IN3 19 
#define IN4 21 

void setup(){ 

pinMode(IN1, OUTPUT); 
pinMode(IN2, OUTPUT); 
pinMode(IN3, OUTPUT); 
pinMode(IN4, OUTPUT); 

digitalWrite(IN1, HIGH); 
digitalWrite(IN2, LOW); 
digitalWrite(IN3, HIGH); 
digitalWrite(IN4, LOW); 
} 

void loop(){ 
}

// The project is still in its early stage – currently we are powering the motors.
// The control unit is an ESP32.
// The motors are driven by the DRV8833 driver.
