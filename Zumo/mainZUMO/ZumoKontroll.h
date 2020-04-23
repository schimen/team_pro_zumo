#ifndef ZumoKontroll_h
#define ZumoKontroll_h

#include <Arduino.h>
#include <Zumo32U4.h>
#include "Definitions.h"

class ZumoKontroll  {
  public:
    ZumoKontroll();
    void setupGyro();
    void updateGyro();
    void calibrateSensors();
    void chargeBattery(uint32_t chargeTime);
    void checkIfTurned();
    void checkBatteryHealth();
    void updateMaxSpeed();
    void updateTotalDistance();
    void updateBatteryPercent();
    void updateTimeOverSeventyPercent();
    void resetAverageSpeed();
    void resetEachSecond();
    void calculateCharge();
    bool isTurned();
    bool isNewCharge();
    bool isLowBattery();
    float getTimeDriven();
    float getAverageSpeed();
    float getNewDistance();
    float getDistanceDriven();
    float getTotalDistance();
    float getSpeed();
    float getMaxSpeed();
    float getBatteryPercent();
    uint32_t getTimeOverSeventyPercent();


    //public variables:
    uint16_t maxSpeed;
    float batteryCharged;

    //zumo objects:
    Zumo32U4Buzzer      buzzer;
    Zumo32U4ButtonA     buttonA;
    Zumo32U4ButtonB     buttonB;
    Zumo32U4ButtonC     buttonC;
    Zumo32U4LineSensors lineSensors;
    Zumo32U4Motors      motors;
    Zumo32U4Encoders    encoders;
    L3G                 gyro;

  private:
    //private variables:
    uint8_t batteryPercent;
    uint16_t countsSpeed;
    uint32_t timeOverSeventyPercent;
    uint32_t previousResetTime;
    float batteryLeft;
    float batteryChargeCycles;
    float batteryChargedTotal;
    float distanceTotal;
    float cumulativeSpeed;
    float measuredMaxSpeed;
    bool newCharge;
    bool batteryServiceNeeded;
    bool batteryChangeNeeded;
    bool turnSense;
    bool seventyCalc;
};

#endif
