
#include "Servo.h"

#define SERVO_FREQUENCY 50
#define SERVO_CHANNEL 0
#define PWM_RESOLUTION 16

const uint16_t servoLeft = 7200;
const uint16_t servoRight = 1850;
uint16_t servoPosition;

void servoSetup(uint8_t servoPin) {
  /*
  neccessary setup for using servo
  */
  ledcSetup(SERVO_CHANNEL, SERVO_FREQUENCY, PWM_RESOLUTION);
  ledcAttachPin(servoPin, SERVO_CHANNEL);
  servoPosition = servoLeft;
  ledcWrite(SERVO_CHANNEL, servoPosition);
}

uint8_t getDegree(uint16_t dutyCycle) {
  /*
  takes the current duty cycle and maps it between 0 and 180 degrees
  */
  uint8_t minDegree = 0;
  uint8_t maxDegree = 180;
  return map(dutyCycle, servoRight, servoLeft, minDegree, maxDegree);
}

void servoTest() {
  /*
  test servo
  */
  uint16_t speed = 10;
  uint8_t  step = (servoLeft - servoRight) / 180;

  for (uint16_t i = servoRight; i < servoLeft; i = i + step) {
    ledcWrite(SERVO_CHANNEL, i);
    Serial.println(getDegree(i));
    delay(speed);
  }
  for (uint16_t i = servoLeft; i > servoRight; i = i - step) {
    ledcWrite(SERVO_CHANNEL, i);
    Serial.println(getDegree(i));
    delay(speed);
  }
}

void servoMoveOneDegree() {
  ledcWrite(SERVO_CHANNEL, servoPosition);
}
