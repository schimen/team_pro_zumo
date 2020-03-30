/*
Class for sensors used in Sensornode

todo:
  - make actual good comments
  - clean up ugly code
*/

#include "Sensor.h"

Sensor::Sensor(uint8_t sensorPin) {
  /*
  generator
  */
  pin = sensorPin;
  readCount = 0;
  cumulativeValue = 0;
  newAverage = false;
}

void Sensor::read() {
  /*
  read sensor
  */
  uint16_t value = analogRead(pin);
  readCount++;
  cumulativeValue = cumulativeValue + sensorValue;
  if (readCount >= averageCount)  {
    newAverage = true;
  }
  if (sensorValue > maxValue) {
    maxValue = value;
  }
  else if (sensorValue < minValue)  {
    minValue = value;
  }
  sensorValue = value;
}

uint16_t Sensor::getValue() {
  /*
  return value
  */
  return sensorValue;
}

uint16_t Sensor::getAverage(bool resetCount) {
  /*
  return value and reset readcount and cumulative value
  if resetcount is true
  */
  uint16_t average = cumulativeValue / readCount;
  if (resetCount) {
    readCount = 0;
    cumulativeValue = 0;
    newAverage = false;
  }
  return average;
}

uint16_t Sensor::getMin() {
  /*
  return minimum value and
  set minimum value to max possible value
  */
  uint16_t value = minValue;
  minValue = MAX_READ;
  return value;
}

uint16_t Sensor::getMax() {
  /*
  return max value and set max value
  to minimum possible value
  */
  uint16_t value = maxValue;
  maxValue = MIN_READ;
  return value;
}
