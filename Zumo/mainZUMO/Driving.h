//* * * * * * * * * * *     Driving functions     * * * * * * * * * * *


#ifndef Driving_h
#define Driving_h

#include <Arduino.h>
#include "ZumoKontroll.h"
#include "Definitions.h"

void followLine(ZumoKontroll *zumo);

void manualDriving(ZumoKontroll *zumo, char inChar);

void patternDriving(ZumoKontroll *zumo);

void driveSquare(ZumoKontroll *zumo);

void driveCircle(ZumoKontroll *zumo);

void forwardAndBack(ZumoKontroll *zumo);

#endif
