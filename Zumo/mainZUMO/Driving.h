#ifndef Driving_h
#define Driving_h

#include <Arduino.h>
#include "ZumoKontroll.h"

#define NUM_SENSORS 5

const float PERPENDICULAR = 655;  //90 graders sving
const float ONE_ROTATION = 909.7;  //ca.11.8cm
const float ONE_METER = 7750;

void followLine(ZumoKontroll *zumo);

void manualDriving(ZumoKontroll *zumo);

void patternDriving(ZumoKontroll *zumo);

void driveSquare(ZumoKontroll *zumo);

void driveCircle(ZumoKontroll *zumo);

void forwardAndBack(ZumoKontroll *zumo);

#endif



/*

sendDetailed(&sensor);

void sendDetailed(Sensor *sensor, bool reset) {

  uint16_t value = sensor->getValue();
  Blynk.virtualWrite(CURRENT_VALUE_PIN, value); //send read value
  terminal.print("  value: ");  //print read value
  terminal.println(value);      //on terminal
  if (sensor->isNewAverage()) {
    uint16_t average = sensor->getAverage(reset);
    Blynk.virtualWrite(AVERAGE_VALUE_PIN, average); //send average value
    terminal.print("average: ");  //print average value
    terminal.println(average);    //on terminal
  }
}
*/
