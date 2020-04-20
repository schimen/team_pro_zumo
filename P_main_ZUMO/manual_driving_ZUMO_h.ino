unsigned long chargeTime;

void manualDriving() {
  switch (inChar) {
    case 'W':
      while (inChar == 'W') {
        motors.setSpeeds(maxSpeed, maxSpeed);
        speedometer();
        inChar = Serial1.read();
      }
      speedometer();
      delay(20);
      break;
    case 'A':
      while (inChar == 'A') {
        motors.setSpeeds(-150, 150);
        speedometer();
        inChar = Serial1.read();
      }
      speedometer();
      delay(20);
      break;
    case 'S':
      while (inChar == 'S') {
        motors.setSpeeds(-maxSpeed, -maxSpeed);
        speedometer();
        inChar = Serial1.read();
      }
      speedometer();
      delay(20);
      break;
    case 'D':
      while (inChar == 'D') {
        motors.setSpeeds(150, -150);
        speedometer();
        inChar = Serial1.read();
      }
      speedometer();
      delay(50);
      break;

    case 'C':
      chargeTime = millis();
      while (inChar != '0') {
        motors.setSpeeds(200, -200);
        inChar = Serial1.read();
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
