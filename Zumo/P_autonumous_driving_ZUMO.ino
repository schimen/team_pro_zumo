/* Author: Line
Denne koden inneholder funksjoner for forhåndsdefinerte
kjøremønster.
 * Autonom mønsterkjøring:
  - Firkant
  - Sirkel
  - Rett fram snu, kjør tilbake

Todo:
Slå sammen med alle andre funksjoner for Zumo, og lag bibliotek
  */ //

#include <Zumo32U4.h>

Zumo32U4LCD lcd;
Zumo32U4ButtonA buttonA;
Zumo32U4Motors motors;
Zumo32U4Encoders encoder; //ca. 4.25m før overflow

const int maxSpeed = 200;
const float PERPENDICULAR = 655;  //90 graders sving
const float ONE_ROTATION = 909.7;  //ca.11.8cm
const float ONEMETER = 7750;
float left;
float right;
float prevLeft;
float prevRight;

void drive_square() {
  lcd.clear();
  lcd.print("Square");

  for (int n = 0; n < 4; n++) {
    motors.setSpeeds(maxSpeed, maxSpeed);
    delay(1000);
    motors.setSpeeds(0, 0);
    delay(200);

    prevLeft = encoder.getCountsLeft();
    prevRight = encoder.getCountsRight();

    motors.setSpeeds(maxSpeed, -maxSpeed);
    while ((left <= prevLeft + PERPENDICULAR) && (right <= prevRight + PERPENDICULAR)) {
      left = encoder.getCountsLeft();
      right = encoder.getCountsRight();
    }
    motors.setSpeeds(0, 0);
    delay(200);
  }
}

void drive_circle() {
  prevRight = encoder.getCountsRight();
  motors.setSpeeds(maxSpeed, maxSpeed / 3);
  while (right <= prevRight + ONEROTATION * 3.09 ) {
    right = encoder.getCountsRight();
  }
  motors.setSpeeds(0, 0);
  delay(200);
}

void forward_and_back() {
  motors.setSpeeds(maxSpeed, maxSpeed);
  unsigned long time_now = millis();

  while (millis() <= time_now + 1000) //kjører 1 sek fram før den snur
  motors.setSpeeds(0, 0);
  delay(200);

  prevLeft = encoder.getCountsLeft();
  prevRight = encoder.getCountsRight();

  motors.setSpeeds(maxSpeed, -maxSpeed);
  while ((left <= prevLeft + PERPENDICULAR * 2) && (right <= prevRight + PERPENDICULAR * 2)) { //Snur 180 grader
    left = encoder.getCountsLeft();
    right = encoder.getCountsRight();
  }
  motors.setSpeeds(0, 0);
  delay(200);

  motors.setSpeeds(maxSpeed, maxSpeed);
  time_now = millis();
  while (millis() <= time_now + 1000) //kjører 1 sek fram
  motors.setSpeeds(0, 0);
  delay(200);
}

void setup() {
  encoder.getCountsAndResetLeft();
  encoder.getCountsAndResetRight();
  delay(4000);
 // forward_and_back();
}

void loop() {

}
