/*
  I made this for easier sharing of objects between
  libraries and main sketch. Also to clean up a bit
  in main sketch
*/

#ifndef zumoObjects_h
#define zumoObjects_h

#include <Zumo32U4.h>

static Zumo32U4Buzzer      buzzer;
static Zumo32U4ButtonA     buttonA;
static Zumo32U4ButtonB     buttonB;
static Zumo32U4ButtonC     buttonC;
static Zumo32U4LineSensors lineSensors;
static Zumo32U4Motors      motors;
static Zumo32U4Encoders    encoders;
static L3G                 gyro;

#endif
