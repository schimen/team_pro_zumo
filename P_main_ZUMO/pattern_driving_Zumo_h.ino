const float PERPENDICULAR = 655;  //90 graders sving
const float ONE_ROTATION = 909.7;  //ca.11.8cm
const float ONE_METER = 7750;
bool btnAIsPressed = false;
bool btnBIsPressed = false;
bool btnCIsPressed = false;

void patternDriving() {
  btnAIsPressed = buttonA.isPressed();
  btnBIsPressed = buttonB.isPressed();
  btnCIsPressed = buttonC.isPressed();

  if (btnAIsPressed == true) driveSquare();
  if (btnBIsPressed == true) driveCircle();
  if (btnCIsPressed == true) forwardAndBack();
}

void driveSquare() {
  Serial.print("Square");
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
}

void driveCircle() {
  Serial.print("circle");
  float right;
  float prevRight = encoders.getCountsRight();
  motors.setSpeeds(maxSpeed, maxSpeed / 3);
  while (right <= (prevRight + ONE_ROTATION * 3.09 )) {
    right = encoders.getCountsRight();
  }
  motors.setSpeeds(0, 0);
  delay(200);
}

void forwardAndBack() {
  Serial.print("forwardAndBack");
  float left;
  float right;
  motors.setSpeeds(maxSpeed, maxSpeed);
  unsigned long time_now = millis();

  while (millis() <= time_now + 1000) //kjører 1 sek fram før den snur
  motors.setSpeeds(0, 0);
  delay(200);

  float prevLeft = encoders.getCountsLeft();
  float prevRight = encoders.getCountsRight();
  
  motors.setSpeeds(maxSpeed, -maxSpeed);
  while ((left <= prevLeft + PERPENDICULAR * 2) && (right <= prevRight + PERPENDICULAR * 2)) { //Snur 180 grader
    left = encoders.getCountsLeft();
    right = encoders.getCountsRight();
  }
  motors.setSpeeds(0, 0);
  delay(200);

  motors.setSpeeds(maxSpeed, maxSpeed);
  time_now = millis();
  while (millis() <= time_now + 1000) //kjører 1 sek fram
  motors.setSpeeds(0, 0);
  delay(200);
}
