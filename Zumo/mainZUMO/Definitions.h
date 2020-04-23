#ifndef Definitions_h
#define Definitions_h

//----Fra Driving.h----
#define NUM_SENSORS 5
#define PERPENDICULAR 655  //90 graders sving
#define ONE_ROTATION 909.7  //ca.11.8cm
#define ONE_METER 7750

//----Fra Zumokontroll.cpp, laget indexer for skriving til ESP----
#define currentSpeedToESP 1
#define distanceTotalToEsP 2
#define speedSixtyFinalToESP 3
#define distanceSixtyFinalToESP 4
#define measuredMaxSpeedToESP 5
#define newBatteryPercentToESP 6
#define lowBatteryToESP 7

//----Fra gyro.h----
#define SERVICE 1         //Utført service på batteriet
#define CHANGED 2          //Utført bytte av batteri

//----Fra Simen----
#define SERVICE 1
#define CHANGED 2
#define BATTERY_MAX 100
#define SEVENTY_LIMIT 0.462
#define SPEED_CONVERSION 7750

//----Fra lineFollower.h----
#define NUM_SENSORS 5

#endif
