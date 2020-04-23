/* * * * * * * * * * *     Driving functions     * * * * * * * * * * *
todo:
  - luk opp magiske tall og legg dem i definitions fil
  - sjekk at alt funker
  - espen og line: gå over kommentarer og se om alt ser bra ut
  - simen: sjekk om du kan effektivisere noe her
*/

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
