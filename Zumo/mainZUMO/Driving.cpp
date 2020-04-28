#include "Driving.h"
#include "beautifulMusic.h"

void followLine(ZumoKontroll *zumo) {
  //Only used in this function
  static unsigned int lineSensorValues[NUM_SENSORS];
  static int lastError = 0;

  int position = zumo->lineSensors.readLine(lineSensorValues);

  int error = position - 2000; // Calculated the margin of error from the centre
  //Uses the margin of error to put a speed difference on the motors, based on curren- and precious error

  int16_t speedDifference = error / 4 + 6 * (error - lastError);
  lastError = error;
  int leftSpeed = zumo->maxSpeed + speedDifference; // Sets the motor speed
  int rightSpeed = zumo->maxSpeed - speedDifference;

  leftSpeed = constrain(leftSpeed, 0, zumo->maxSpeed); //Sets a "roof" for the speed in both ends, so the speed is between 0 and 400
  rightSpeed = constrain(rightSpeed, 0, zumo->maxSpeed);

  zumo->motors.setSpeeds(leftSpeed, rightSpeed); // Changes the motors speed
}

void manualDriving(ZumoKontroll *zumo, char inChar) {
/*
This function handles the input from the ESP. The characters printed defines the actions
for moving, charging and setting max speed.
*/
  switch (inChar) {
    case 'W': //This case makes the car drive forward, at max speed
      while (inChar == 'W') {
        zumo->motors.setSpeeds(zumo->maxSpeed, zumo->maxSpeed);
        inChar = Serial1.read();
      }
      delay(20);
      break;
    case 'A': //This case turns the car to the left at a fixed speed
      while (inChar == 'A') {
        zumo->motors.setSpeeds(-150, 150);
        inChar = Serial1.read();
      }
      delay(20);
      break;
    case 'S': //This case makes the car drive backwards, at max speed
      while (inChar == 'S') {
        zumo->motors.setSpeeds(-(zumo->maxSpeed), -(zumo->maxSpeed));
        inChar = Serial1.read();
      }
      delay(20);
      break;
    case 'D': //This case turns the car to the right at a fixed speed
      while (inChar == 'D') {
        zumo->motors.setSpeeds(150, -150);
        inChar = Serial1.read();
      }
      delay(50);
      break;

    case 'C': //This case measures the time the charge button is held, and is used to charge the battery
      uint32_t chargeTime = millis();
      while (inChar != '0') {
        zumo->motors.setSpeeds(200, -200);
        inChar = Serial1.read();
      }
      zumo->chargeBattery(chargeTime);
      zumo->motors.setSpeeds(0, 0);
      delay(50);
      break;

    case '0':
      zumo->motors.setSpeeds(0, 0);
      delay(20);
      break;
  } //end switch
}

void patternDriving(ZumoKontroll *zumo) {
/*
This function checks for physical button pushing, and calls
the pattern driving accordingly.
*/
  bool btnAIsPressed = zumo->buttonA.getSingleDebouncedRelease();
  bool btnBIsPressed = zumo->buttonB.getSingleDebouncedRelease();
  bool btnCIsPressed = zumo->buttonC.getSingleDebouncedRelease();

  if (btnAIsPressed == true) {
    delay(500);
    driveSquare(zumo);
  }
  if (btnBIsPressed == true) {
    delay(500);
    driveCircle(zumo);
  }
  if (btnCIsPressed == true) {
    delay(500);
    forwardAndBack(zumo);
  }
}

void driveSquare(ZumoKontroll *zumo) {
/*
This function is one of three preinstalled patters for automatic Driving.
This is the Square-driving.
*/
  Serial.print("GoT");
  zumo->buzzer.playFromProgramSpace(GoT);
  float left;
  float right;
  for (int n = 0; n < 4; n++) {
    zumo->motors.setSpeeds(zumo->maxSpeed, zumo->maxSpeed);
    delay(1000);
    zumo->motors.setSpeeds(0, 0);
    delay(200);

    float prevLeft = zumo->encoders.getCountsLeft();
    float prevRight = zumo->encoders.getCountsRight();

    zumo->motors.setSpeeds(zumo->maxSpeed, -(zumo->maxSpeed));
    while ((left <= prevLeft + PERPENDICULAR) && (right <= prevRight + PERPENDICULAR)) {
      left = zumo->encoders.getCountsLeft();
      right = zumo->encoders.getCountsRight();
    }
    zumo->motors.setSpeeds(0, 0);
    delay(200);
  }
  while (zumo->buzzer.isPlaying());
}

void driveCircle(ZumoKontroll *zumo) {
  /*
  This function is one of three preinstalled patters for automatic Driving.
  This is the circle-driving.
  */
  Serial.print("circle");
  zumo->buzzer.playFromProgramSpace(BlueDanube);
  float right;
  float prevRight = zumo->encoders.getCountsRight();
  zumo->motors.setSpeeds(zumo->maxSpeed, zumo->maxSpeed / 3);
  while (right <= (prevRight + ONE_ROTATION * 3.09 )) {
    right = zumo->encoders.getCountsRight();
  }
  zumo->motors.setSpeeds(0, 0);
  delay(200);
  while (zumo->buzzer.isPlaying());
}

void forwardAndBack(ZumoKontroll *zumo) {
  /*
  This function is one of three preinstalled patters for automatic Driving.
  This is the "forwards-then-back-again"-driving.
  */
  zumo->maxSpeed = 200;
  Serial.print("forwardAndBack");
  zumo->buzzer.playFromProgramSpace(ImperialMarch);
  float left;
  float right;
  zumo->motors.setSpeeds(zumo->maxSpeed, zumo->maxSpeed);
  unsigned long time_now = millis();
  Serial.println(time_now);
  while (millis() <= time_now + 1000) //Drives for one second before turning
  Serial.println("time done");
  zumo->motors.setSpeeds(0, 0);
  delay(200);

  float prevLeft = zumo->encoders.getCountsLeft();
  float prevRight = zumo->encoders.getCountsRight();

  zumo->motors.setSpeeds(zumo->maxSpeed, -(zumo->maxSpeed));
  Serial.println("turn");
  while ((left <= prevLeft + PERPENDICULAR * 2) && (right <= prevRight + PERPENDICULAR * 2)) { //Turn 180 degrees
    left = zumo->encoders.getCountsLeft();
    right = zumo->encoders.getCountsRight();
  }
  zumo->motors.setSpeeds(0, 0);
  delay(200);

  zumo->motors.setSpeeds(zumo->maxSpeed, zumo->maxSpeed);
  Serial.println("back");
  time_now = millis();
  while (millis() <= time_now + 1000) //Drives for one second
  zumo->motors.setSpeeds(0, 0);
  Serial.println("done");
  while (zumo->buzzer.isPlaying());
  delay(200);
}
