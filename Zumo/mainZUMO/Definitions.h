#ifndef Definitions_h
#define Definitions_h

//----From Driving.h----
#define NUM_SENSORS 5
#define PERPENDICULAR 655  // Encoder value for 90 degree turn
#define ONE_ROTATION 909.7  //Approx. 11.8cm
#define ONE_METER 7750     // Encoder value for one meter

//----From Zumokontroll.cpp, made indexes for writing to the ESP----
#define currentSpeedToESP 1
#define distanceTotalToEsP 2
#define speedSixtyFinalToESP 3
#define distanceSixtyFinalToESP 4
#define measuredMaxSpeedToESP 5
#define newBatteryPercentToESP 6
#define lowBatteryToESP 7
#define chargeCyclesToESP 8
#define seventyTimeToESP 9

//----From gyro.h----
#define SERVICE 1         //Completed a battery service
#define CHANGED 2         //Completed a battery change

//----Battery----
#define SERVICE 1
#define CHANGED 2
#define BATTERY_MAX 100
#define SEVENTY_LIMIT 0.462
#define SPEED_CONVERSION 7750 // Encoder value for one meter

//----From lineFollower.h----
#define NUM_SENSORS 5

#endif
