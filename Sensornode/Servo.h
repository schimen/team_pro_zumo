#ifndef Servo_h
#define Servo_h

#include <Arduino.h>

void servoSetup(uint8_t servoPin);
uint8_t getDegree();
void servoTest();

#endif
