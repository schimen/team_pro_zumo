
#include "patternDriving.h"
#include "beautifulMusic.h"

void patternDriving() {
  //skal disse verdiene lagres når funksjonenen slettes?
  //hvis de skal det, bruk static istedet
  bool btnAIsPressed = buttonA.getSingleDebouncedRelease();
  bool btnBIsPressed = buttonB.getSingleDebouncedRelease();;
  bool btnCIsPressed = buttonC.getSingleDebouncedRelease();

  if (btnAIsPressed == true) {
    delay(500);
    driveSquare();
  }
  if (btnBIsPressed == true) {
    delay(500);
    driveCircle();
  }
  if (btnCIsPressed == true) {
    delay(500);
    forwardAndBack();
  }
}

void driveSquare() {
  Serial.print("BlueDanube");
  buzzer.playFromProgramSpace(GoT);
  float left;
  float right;
  for (int n = 0; n < 4; n++) {
    motors.setSpeeds(maxSpeed, maxSpeed);
    delay(1000);
    motors.setSpeeds(0, 0);
    delay(200);

    float prevLeft = encoders.getCountsLeft();
    float prevRight = encoders.getCountsRight();

    motors.setSpeeds(maxSpeed, -maxSpeed);
    while ((left <= prevLeft + PERPENDICULAR) && (right <= prevRight + PERPENDICULAR)) {
      left = encoders.getCountsLeft();
      right = encoders.getCountsRight();
    }
    motors.setSpeeds(0, 0);
    delay(200);
  }
  while (buzzer.isPlaying());
}

void driveCircle() {
  Serial.print("circle");
  buzzer.playFromProgramSpace(BlueDanube);
  float right;
  float prevRight = encoders.getCountsRight();
  motors.setSpeeds(maxSpeed, maxSpeed / 3);
  while (right <= (prevRight + ONE_ROTATION * 3.09 )) {
    right = encoders.getCountsRight();
  }
  motors.setSpeeds(0, 0);
  delay(200);
  while (buzzer.isPlaying());
}

void forwardAndBack() {
  maxSpeed = 200;
  Serial.print("forwardAndBack");
  buzzer.playFromProgramSpace(ImperialMarch);
  float left;
  float right;
  motors.setSpeeds(maxSpeed, maxSpeed);
  unsigned long time_now = millis();
  Serial.println(time_now);
  while (millis() <= time_now + 1000) //kjører 1 sek fram før den snur
  Serial.println("time done");
  motors.setSpeeds(0, 0);
  delay(200);

  float prevLeft = encoders.getCountsLeft();
  float prevRight = encoders.getCountsRight();

  motors.setSpeeds(maxSpeed, -maxSpeed);
  Serial.println("turn");
  while ((left <= prevLeft + PERPENDICULAR * 2) && (right <= prevRight + PERPENDICULAR * 2)) { //Snur 180 grader
    left = encoders.getCountsLeft();
    right = encoders.getCountsRight();
  }
  motors.setSpeeds(0, 0);
  delay(200);

  motors.setSpeeds(maxSpeed, maxSpeed);
  Serial.println("back");
  time_now = millis();
  while (millis() <= time_now + 1000) //kjører 1 sek fram
  motors.setSpeeds(0, 0);
  Serial.println("done");
  while (buzzer.isPlaying());
  delay(200);
}
