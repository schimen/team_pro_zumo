#include "manualDriving.h"


void manualDriving(char input) {
  switch (input) {
    case 'W':
      while (input == 'W') {
        motors.setSpeeds(maxSpeed, maxSpeed);
        speedometer();
        input = Serial1.read();
      }
      speedometer();
      delay(20);
      break;
    case 'A':
      while (input == 'A') {
        motors.setSpeeds(-150, 150);
        speedometer();
        input = Serial1.read();
      }
      speedometer();
      delay(20);
      break;
    case 'S':
      while (input == 'S') {
        motors.setSpeeds(-maxSpeed, -maxSpeed);
        speedometer();
        input = Serial1.read();
      }
      speedometer();
      delay(20);
      break;
    case 'D':
      while (input == 'D') {
        motors.setSpeeds(150, -150);
        speedometer();
        input = Serial1.read();
      }
      speedometer();
      delay(50);
      break;

    case 'C':
      chargeTime = millis();
      while (input != '0') {
        motors.setSpeeds(200, -200);
        input = Serial1.read();
      }
      chargeBattery();
      motors.setSpeeds(0, 0);
      speedometer();
      delay(50);
      break;

    case '0':
      motors.setSpeeds(0, 0);
      speedometer();
      delay(20);
      break;
  } //end switch
} //end func
