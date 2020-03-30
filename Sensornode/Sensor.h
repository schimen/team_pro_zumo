
#ifndef Sensor_h
#define Sensor_h
#define MAX_READ 4096
#define MIN_READ 0

#include <Arduino.h>

class Sensor {
  public:
    Sensor(uint8_t sensorPin);
    void     read();
    uint16_t getValue();
    uint16_t getAverage(bool resetCount=true);
    bool     newAverage;
    bool     newMaxMin;
    static uint16_t getMax();
    static uint16_t getMin();
    static uint8_t  averageCount;
    static uint16_t maxValue;
    static uint16_t minValue;

  private:
    uint8_t  pin;
    uint8_t  readCount;
    uint16_t sensorValue;
    uint32_t cumulativeValue;
};

#endif
