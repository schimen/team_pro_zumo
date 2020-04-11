/* * * * * * * * * * *     Sensor class     * * * * * * * * * * */

#ifndef Sensor_h
#define Sensor_h

#include <Arduino.h>

class Sensor {
  public:

    //constructor of Sensor class
    //                   pin, max allowed for sensor
    Sensor(uint8_t sensorPin, uint16_t max);

    //read sensor.
    //on read, check if over max, check for new average and store read value
    void read();

    //return sensor value
    uint16_t getValue();

    //return average value (cumulativeValue / readCount)
    //bool reset will reset used variables if true
    //          (readCount, cumulativeValue and newAverage)
    uint16_t getAverage(bool reset = true);

    //return max value (maxValue)
    //bool reset will set maxValue to minimum if true
    static uint16_t getMax(bool reset = true);

    //return min value (minValue)
    //bool reset will set minValue to maximum if true
    static uint16_t getMin(bool reset = true);

    //returns true if new average is ready
    bool isNewAverage();

    //returns true if 2 or more sensors are over allowed value
    static bool isAlarm();

    //public variables
    static uint8_t  sensorsOverMax; //number of sensors over maxAllowed value
    static uint8_t  averageCount;   //number of reads for new average value
    static uint16_t maxValue; //maximum value of all sensors
    static uint16_t minValue; //minimum value of all sensors

  private:
    bool     newAverage;  //true if new average value is available
    bool     overMax; //signal if sensor is over max allowed value
    uint8_t  pin;
    uint8_t  readCount; //number of reads of sensor (resets on getAverage())
    uint16_t sensorValue; //current sensor value
    uint16_t maxAllowed;  //maximum value allowed for sensor
    uint32_t cumulativeValue; //cumulative sensor value (reset on getAverage())
};

#endif
