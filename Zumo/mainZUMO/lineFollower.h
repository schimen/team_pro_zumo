/* * * * * * * * * * *     Line follower functions     * * * * * * * * * * */

#ifndef LineFollower_h
#define LineFollower_h

#include <Arduino.h>
#include <Zumo32U4Motors.h>
#include "zumoObjects.h"
#include "speedometerBattery.h"

#define NUM_SENSORS 5

unsigned int lineSensorValues[NUM_SENSORS];
int lastError = 0;

void calibrateSensors();

void followLine();

#endif
