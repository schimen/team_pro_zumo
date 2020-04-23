#include "Driving.h"
#include "beautifulMusic.h"

void followLine(ZumoKontroll *zumo) {
  //brukes bare i denne funksjonen:d
  static unsigned int lineSensorValues[NUM_SENSORS];
  static int lastError = 0;

  int position = zumo->lineSensors.readLine(lineSensorValues);

  int error = position - 2000; // regner ut feilmargin fra sentrum
  //Bruker feilmarginen til å sette en fartsforskjell på motorene basert på nåværende og forrige feilmargin
  int16_t speedDifference = error / 4 + 6 * (error - lastError);
  lastError = error;
  int leftSpeed = zumo->maxSpeed + speedDifference; // Setter motorhastigheter
  int rightSpeed = zumo->maxSpeed - speedDifference;

  leftSpeed = constrain(leftSpeed, 0, zumo->maxSpeed); //Setter et "tak" for hastighetene i begge ender, om farten er under 0 eller over 400
  rightSpeed = constrain(rightSpeed, 0, zumo->maxSpeed);

  zumo->motors.setSpeeds(leftSpeed, rightSpeed); // Endrer hastigheten til motorene
}

void manualDriving(ZumoKontroll *zumo, char inChar) {
  switch (inChar) {
    case 'W':
      while (inChar == 'W') {
        zumo->motors.setSpeeds(zumo->maxSpeed, zumo->maxSpeed);
        inChar = Serial1.read();
      }
      delay(20);
      break;
    case 'A':
      while (inChar == 'A') {
        zumo->motors.setSpeeds(-150, 150);
        inChar = Serial1.read();
      }
      delay(20);
      break;
    case 'S':
      while (inChar == 'S') {
        zumo->motors.setSpeeds(-(zumo->maxSpeed), -(zumo->maxSpeed));
        inChar = Serial1.read();
      }
      delay(20);
      break;
    case 'D':
      while (inChar == 'D') {
        zumo->motors.setSpeeds(150, -150);
        inChar = Serial1.read();
      }
      delay(50);
      break;

    case 'C':
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
  //skal disse verdiene lagres når funksjonenen slettes?
  //hvis de skal det, bruk static istedet
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
  Serial.print("BlueDanube");
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
  zumo->maxSpeed = 200;
  Serial.print("forwardAndBack");
  zumo->buzzer.playFromProgramSpace(ImperialMarch);
  float left;
  float right;
  zumo->motors.setSpeeds(zumo->maxSpeed, zumo->maxSpeed);
  unsigned long time_now = millis();
  Serial.println(time_now);
  while (millis() <= time_now + 1000) //kjører 1 sek fram før den snur
  Serial.println("time done");
  zumo->motors.setSpeeds(0, 0);
  delay(200);

  float prevLeft = zumo->encoders.getCountsLeft();
  float prevRight = zumo->encoders.getCountsRight();

  zumo->motors.setSpeeds(zumo->maxSpeed, -(zumo->maxSpeed));
  Serial.println("turn");
  while ((left <= prevLeft + PERPENDICULAR * 2) && (right <= prevRight + PERPENDICULAR * 2)) { //Snur 180 grader
    left = zumo->encoders.getCountsLeft();
    right = zumo->encoders.getCountsRight();
  }
  zumo->motors.setSpeeds(0, 0);
  delay(200);

  zumo->motors.setSpeeds(zumo->maxSpeed, zumo->maxSpeed);
  Serial.println("back");
  time_now = millis();
  while (millis() <= time_now + 1000) //kjører 1 sek fram
  zumo->motors.setSpeeds(0, 0);
  Serial.println("done");
  while (zumo->buzzer.isPlaying());
  delay(200);
}
