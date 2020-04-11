#include "SensornodePWM.h"
#include "Definitions.h"

PWM::PWM(uint8_t chan, uint8_t pin, double freq, uint8_t res) {
  /*
  constructor for class. save channel and frequency.
  set up channel and attach pin to channel
  (default freq = 50 because it is a common PWM frequency
   default res = 8   because it is a common PWM resolution)
  */
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
  return dutyCycle
  */
  return dutyCycle;
}

uint8_t PWM::getDegree() {
  /*
  takes the current duty cycle and maps it between min and max degrees
  */
  return map(dutyCycle, SERVO_LEFT, SERVO_RIGHT, MIN_DEGREE, MAX_DEGREE);
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

bool PWM::isOn()  {
  /*
  check if duty cycle on channel is more than zero and return true/false
  */
  if (ledcRead(channel) > 0)  {
    return true;
  }
  else  {
    return false;
  }
}
