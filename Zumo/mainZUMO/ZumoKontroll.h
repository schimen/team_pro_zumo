#ifndef ZumoKontroll_h
#define ZumoKontroll_h

#include <Arduino.h>
#include <Zumo32U4.h>

class ZumoKontroll  {
  public:
    ZumoKontroll();
    void update(); //tar over for speedometer()
    void checkIfTurned();
    void updateGyro();
    void calibrateSensors();

    //public variables:
    int maxSpeed;

  private:
    //zumo objects:
    Zumo32U4Buzzer      buzzer;
    Zumo32U4ButtonA     buttonA;
    Zumo32U4ButtonB     buttonB;
    Zumo32U4ButtonC     buttonC;
    Zumo32U4LineSensors lineSensors;
    Zumo32U4Motors      motors;
    Zumo32U4Encoders    encoders;
    L3G                 gyro;


    //fikse noen nye navn?
    void oneCheck();
    void speedCheck();
    void sixtyCheck();
    void seventyCheck();
    void batteryCheck();
    void batteryHealth();
    void writeToESP();  //why?

    //private variables:
    float batteryChargeCycles;
    float batteryLeft;
    float batteryChargedTotal;
    float speedSixtyFinal;
    float distanceSixtyFinal;
    float distanceTotal;
    float speedSixty;
    float avgSpeedSixty;
    float speedo;
    float measuredMaxSpeed;
    float speedLeft;
    float speedRight;
    float distanceSixty;
    float charged;
    bool oneCalc;
    bool newCharge;
    bool newSpeedo;
    bool newDistanceTotal;
    bool newSpeedSixtyFinal;
    bool newDistanceSixtyFinal;
    bool newMaxSpeed;
    bool newBatteryPercent;
    bool lowBatteryToESP;
    bool toggleLED;
    bool sixtyCalc;
    bool seventyCalc;
    bool lowBattery;
    bool batteryServiceNeeded;
    bool prevServiceState;
    bool batteryChangeNeeded;
    bool prevChangeState;
    bool turnSense;
    unsigned long chargeTime;
    unsigned long TimeInterval;
    unsigned long startTime;
    unsigned long movementTime;
    unsigned long currentMillis;
    unsigned long seventyTime;
    unsigned long sixtyCalcTime;
    unsigned long seventyCalcTime;
    int yAxis;
    int batteryCapasity;
    int workDone;
    int numSixty;
    int batteryPercent;
    const int SERVCE = 1;
    const int CHANGED = 2;
    const int BATTERY_MAX = 100;
    const float SEVENTY_LIMIT = 0.462;
};


#endif
