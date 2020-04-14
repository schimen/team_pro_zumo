#include "SensornodePWM.h"
#include "Definitions.h"

PWM::PWM(uint8_t chan, uint8_t pin, uint8_t res, double freq) {
  /*
  constructor for class. save channel and frequency.
  set up channel and attach pin to channel
  (default res = 8   because it is a common PWM resolution
   default freq = 50 because it is a common PWM frequency)
  */
  endPoint = SERVO_RIGHT;
  channel = chan;
  frequency = freq;
  ledcSetup(channel, frequency, res);
  ledcAttachPin(pin, channel);
}

void PWM::setDuty(uint16_t newDuty) {
  /*
  save and write the duty cycle to the channel
  */
  dutyCycle = newDuty;
  ledcWrite(channel, dutyCycle);
}

void PWM::setDegree(uint8_t degree) {
  /*
  convert degree to duty cycle and use setDuty function to set new duty cycle
  */
  setDuty(map(degree, MIN_DEGREE, MAX_DEGREE, SERVO_LEFT, SERVO_RIGHT));
}

void PWM::setTone(double freq) {
  /*
  sets a frequency to the buzzer
  */
  frequency = freq;
  ledcWriteTone(channel, frequency);
}

void PWM::setOff()  {
  /*
  turn off channel
  */
  ledcWrite(channel, 0);
}

void PWM::setEnd()  {
  /*
  sets the servo to the endpoint and alternates the endpoint
  first change the point for end, then set position to new point
  */
  changeEnd();
  setDuty(endPoint);
}

void PWM::changeDuty(int16_t change) {
  /*
  sets new duty cycle as sum of old duty cycle and change
  */
  setDuty(dutyCycle + change);
}

void PWM::changeDegree(int8_t degreeChange) {
  /*
  checks for current degree and moves servo to
  new position based on degreeChange
  */
  uint8_t degree = getDegree();
  setDegree(degree + degreeChange);
}

uint16_t PWM::getDuty()  {
  /*
  return duty cycle of channel
  */
  return ledcRead(channel);
}

uint8_t PWM::getDegree() {
  /*
  takes the current duty cycle and maps it between min and max degrees
  */
  return map(dutyCycle, SERVO_LEFT, SERVO_RIGHT, MIN_DEGREE, MAX_DEGREE);
}

void PWM::changeEnd()  {
  /*
  if endpoint is right, set endpoint to left and opposite
  */
  if (endPoint == SERVO_LEFT) {
    endPoint = SERVO_RIGHT;
  }
  else  {
    endPoint = SERVO_LEFT;
  }
}

bool PWM::isOn()  {
  /*
  check if duty cycle on channel is more than zero and return true/false
  */
  if (getDuty() > 0)  {
    return true;
  }
  else  {
    return false;
  }
}

bool PWM::isEnd() {
  /*
  check if duty cycle is close to the endpoint and return true/false
  */
  if (getDuty() >= (endPoint - SERVO_STEP) and
      getDuty() <= (endPoint + SERVO_STEP))  {
    return true;
  }
  else  {
    return false;
  }
}
