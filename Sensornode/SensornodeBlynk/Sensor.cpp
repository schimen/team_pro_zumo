#include "Sensor.h"
#include "Definitions.h"

Sensor::Sensor(uint8_t sensorPin, uint16_t max) {
  /*
  constructor. saves pin, and maxAllowed
  sets start value for readCount, cumulativeValue and newAverage
  */
  pin = sensorPin;
  maxAllowed = max;
  readCount = 0;
  cumulativeValue = 0;
  newAverage = false;
}

void Sensor::read() {
  /*
  read sensor.
  On read :
    increase readCount, increase cumulativeValue, check for new average,
    check if sensors are over max allowed, check if sensors over max,
    store read value as sensorValue
  */
  uint16_t value = analogRead(pin);

  readCount++;
  cumulativeValue = cumulativeValue + sensorValue;
  if (readCount >= averageCount)  {
    newAverage = true;
  }
  if (sensorValue > maxAllowed and not overMax) {
    sensorsOverMax++;
    overMax = true;
  }
  else if (sensorValue < maxAllowed and overMax) {
    sensorsOverMax--;
    overMax = false;
  }
  if (sensorValue > maxValue) {
    maxValue = value;
  }
  else if (sensorValue < minValue)  {
    minValue = value;
  }
  sensorValue = value;
}

uint16_t Sensor::getValue(bool read) {
  /*
  return value
  */
  if (read) {
    read();
  }
  
  return sensorValue;
}

uint16_t Sensor::getAverage(bool reset) {
  /*
  Return average value (cumulativeValue / readCount)
  Reset readCount, cumulativeValue and newAverage if resetCount is true.
  */
  uint16_t average = cumulativeValue / readCount;
  if (reset) {
    readCount = 0;
    cumulativeValue = 0;
    newAverage = false;
  }
  return average;
}

uint16_t Sensor::getMax(bool reset) {
  /*
  return maximum value
  if reset: set max value to minimum possible value
  */
  uint16_t value = maxValue;
  if (reset)  {
    maxValue = MIN_READ;
  }
  return value;
}

uint16_t Sensor::getMin(bool reset) {
  /*
  return minimum value and
  if reset: set minimum value to maximum possible value
  */
  uint16_t value = minValue;
  if (reset)  {
    minValue = MAX_READ;
  }
  return value;
}

bool Sensor::isNewAverage() {
  /*
  return newAverage
  */
  return newAverage;
}

bool Sensor::isAlarm()  {
  /*
  returns true if number of sensors over max value is 2 or more.
  */
  if (sensorsOverMax >= 2) {
    return true;
  }
  else  {
    return false;
  }
}
