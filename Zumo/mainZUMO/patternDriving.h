/* * * * * * * * * * *     Pattern driving     * * * * * * * * * * */


//fiks max speed

#ifndef patternDriving_h
#define patternDriving_h

#include <Arduino.h>
#include "zumoObjects.h"
#include "speedometerBattery.h"

//fiks maxspeed

const float PERPENDICULAR = 655;  //90 graders sving
const float ONE_ROTATION = 909.7;  //ca.11.8cm
const float ONE_METER = 7750;


void patternDriving();

void driveSquare();

void driveCircle();

void forwardAndBack();

#endif
