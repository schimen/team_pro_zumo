/* * * * * * * * * * *     ZumoKontroll class     * * * * * * * * * * *

todo:
  - sjekk at alt funker
  - fiks kommentarer og magiske tall i .cpp fil

*/

#ifndef ZumoKontroll_h
#define ZumoKontroll_h

#include <Arduino.h>
#include <Zumo32U4.h>
#include "Definitions.h"

class ZumoKontroll  {
  public:

    //constructor
    ZumoKontroll();

    //setup for gyro
    void setupGyro();

    //check if zumo has been turned (?upside down?)
    void checkIfTurned();

    //calibrate zumo sensors
    void calibrateSensors();

    //run when "charge" button in Blynk is held in,
    //and calculates charge percentage.
    //takes start time of button press as argument.
    void chargeBattery(uint32_t chargeTime);

    //checks if battery change or service is needed.
    //saves in batteryChangeNeeded and batteryServiceNeeded varialbles
    void checkBatteryHealth();

    //reset cumulativeSpeed, countsSpeed and timeOverSeventyPercent
    void resetAverageSpeed();

    //reset values used in one-second-interval timer
    //(resets previousResetTime and resets encoders)
    void resetEachSecond();

    //returns newCharge
    bool isNewCharge();

    //returns turnSense
    bool isTurned();

    //returns true if battery percent is lower than a low value.
    //otherwise it returns false
    bool isLowBattery();

    //returns time since last resetEachSecond() call in seconds
    float getTimeDriven();

    //returns average speed.
    //(also resets cumulativeSpeed and countsSpeed)
    float getAverageSpeed();

    //returns distance traveled since last call
    float getNewDistance();

    //returns the distance driven that is saved on the encoders
    float getDistanceDriven();

    //updates and returns the total distance driven
    float getTotalDistance();

    //takes distance driven and time driven, and returns the current speed
    float getSpeed();

    //updates and returns the measured max speed
    float getMaxSpeed();

    //updates and returns the current battery percentage
    float getBatteryPercent();

    //updates and returns amount of time max speed was over 70%
    uint32_t getTimeOverSeventyPercent();

    //public variables:
    uint16_t maxSpeed;        //- current max speed
    float    batteryCharged;  //- ?

    //zumo objects:
    L3G                 gyro;
    Zumo32U4Motors      motors;
    Zumo32U4Buzzer      buzzer;
    Zumo32U4ButtonA     buttonA;
    Zumo32U4ButtonB     buttonB;
    Zumo32U4ButtonC     buttonC;
    Zumo32U4Encoders    encoders;
    Zumo32U4LineSensors lineSensors;

  private:
    void updateGyro();
    void updateCharge();
    void updateMaxSpeed();
    void updateTotalDistance();
    void updateBatteryPercent();
    void updateTimeOverSeventyPercent();

    //private variables:
    uint8_t  batteryPercent;          //- ?
    uint16_t countsSpeed;             //- count for average speed
    uint32_t previousResetTime;       //- timestamp of last resetEachSecond call
    uint32_t timeOverSeventyPercent;  //- time driven over seventy percent of maximum speed
    float    distanceTotal;           //- total distance driven in meter
    float    cumulativeSpeed;         //- cumulative speed for average calculation
    float    measuredMaxSpeed;        //- highest measured speed
    float    batteryLeft;             //- ?
    float    batteryChargeCycles;     //- ?
    float    batteryChargedTotal;     //- ?
    bool     turnSense;               //- ?
    bool     seventyCalc;             //- ?
    bool     newCharge;               //- ?
    bool     batteryServiceNeeded;    //- ?
    bool     batteryChangeNeeded;     //- ?
};

#endif
